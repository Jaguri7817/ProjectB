// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/TargetActor/GATA_AttackHitCheck.h"

#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "Engine/OverlapResult.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h"

AGATA_AttackHitCheck::AGATA_AttackHitCheck()
{
	// 공격 판정은 서버에서만 생성하도록
	ShouldProduceTargetDataOnServer = true;

	// 타겟 확정 이후 TargetActor 제거
	bDestroyOnConfirmation = true;

}

void AGATA_AttackHitCheck::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	UE_LOG(LogTemp, Warning, TEXT("[TargetActor] Starting Targeting | NetMode: %d"), static_cast<int32>(GetNetMode()));

	if (!Ability)
	{
		return;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo)
	{
		SourceActor = ActorInfo->AvatarActor.Get();
	}

	UE_LOG(LogTemp, Warning, TEXT("[TargetActor] SourceActor: %s | Authority: %s"), *GetNameSafe(SourceActor), SourceActor && SourceActor->HasAuthority() ? TEXT("True") : TEXT("False"));
	
};

void AGATA_AttackHitCheck::ConfirmTargetingAndContinue()
{
	UE_LOG(LogTemp, Warning, TEXT("[TargetActor] Confirm Targeting And Continue | Should Produce: %s"), ShouldProduceTargetData() ? TEXT("true") : TEXT("flase"));

	if (!ShouldProduceTargetData())
	{
		return;
	}

	TargetDataReadyDelegate.Broadcast(MakeTargetData());
}

FGameplayAbilityTargetDataHandle AGATA_AttackHitCheck::MakeTargetData() const
{
	UE_LOG(LogTemp, Warning, TEXT("[TargetActor] MakeTargetData ENTER"));

	if (!IsValid(SourceActor))
	{
		return {};
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return {};
	}

	// -------------------------------------
	// 1. 공격 판정 위치 계산
	// -------------------------------------
	const FVector BoxCenter = GetAttackBoxCenter();
	const FVector BoxHalfExtent = GetAttackBoxHalfExtent();

	const FQuat BoxRotation = SourceActor->GetActorQuat();		// 캐릭터가 바라보는 방향에 맞춰 Box도 회전


	// -------------------------------------
	// 2. Collision Query 설정
	// -------------------------------------
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TargetObjectType);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PBMeleeAttack), false, SourceActor);
	QueryParams.AddIgnoredActor(SourceActor);

	// -------------------------------------
	// 3. 공격 범위 내 Actor 검사
	// -------------------------------------
	TArray<FOverlapResult> OverlapResults;

	const bool bHasOverlap = World->OverlapMultiByObjectType(
		OverlapResults,
		BoxCenter,
		BoxRotation,
		ObjectQueryParams,
		FCollisionShape::MakeBox(BoxHalfExtent),
		QueryParams
	);

#if	ENABLE_DRAW_DEBUG
	if (bDebug)
	{
		DrawDebugBox(World, BoxCenter, BoxHalfExtent, BoxRotation, bHasOverlap ? FColor::Green : FColor::Red, false, 1.0f, 0, 2.0f);
	}
#endif

	if (!bHasOverlap)
	{
		return {};
	}

	// -------------------------------------
	// 4. 중복 제거 + 유효 공격 대상 수집
	// -------------------------------------
	TSet<AActor*> UniqueTargets;

	TArray<TWeakObjectPtr<AActor>> TargetActors;
	TargetActors.Reserve(OverlapResults.Num());

	UE_LOG(LogTemp, Warning, TEXT("[TargetActor] Overlap Result: %s | Count: %d"), bHasOverlap ? TEXT("TRUE") : TEXT("FALSE"), OverlapResults.Num());

	for (const auto& OverlapResult : OverlapResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TargetActor] Overlap Result: %s | Count: %d"), bHasOverlap ? TEXT("TRUE") : TEXT("FALSE"), OverlapResults.Num());

		AActor* Candidate = OverlapResult.GetActor();

		if (!IsValidTarget(Candidate))
		{
			continue;
		}

		if (UniqueTargets.Contains(Candidate))
		{
			continue;
		}

		UniqueTargets.Add(Candidate);
		TargetActors.Emplace(Candidate);
	}

	if (TargetActors.IsEmpty())
	{
		return {};
	}

	// -------------------------------------
	// 5. TargetData 생성
	// -------------------------------------
	
	// new 생성이 Effective C++에 어긋나 보이지만 FGameplayAbilityTargetDataHandle의 내부 데이터 저장소에서 TSharedPtr<>이고,
	// 에픽에서도 Add()로 넣는 TargetData를 new로 생성해야 한다고 명시하고 있다.
	// 즉 여기서는 Handle이 소유권을 인수하는 엔진 규약이라 직접 delete하면 안된다.
	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();

	TargetData->TargetActorArray = MoveTemp(TargetActors);

	return FGameplayAbilityTargetDataHandle(TargetData);
}

bool AGATA_AttackHitCheck::IsValidTarget(const AActor* Candidate) const
{
	if (!IsValid(Candidate))
	{
		return false;
	}

	if (Candidate == SourceActor)
	{
		return false;
	}

	// 어떤 클래스가 지정되어 있다면 해당 클래스 계열만 공격 대상으로 인정하도록
	if (RequiredTargetClass && !Candidate->IsA(RequiredTargetClass))
	{
		return false;
	}

	// GAS 자체 Target Filter도 함께 통과시킨다.
	if (!Filter.FilterPassesForActor(Candidate))
	{
		return false;
	}

	return true;
}

FVector AGATA_AttackHitCheck::GetAttackBoxCenter() const
{
	const FVector SourceLocation = SourceActor->GetActorLocation();
	const FVector Forward = SourceActor->GetActorForwardVector();

	// Box의 중심을 공격 거리의 절반맠늠 앞으로 옮긴다.
	// 따라서 Box는 캐릭터 위치로부터 AttackRange만큼 전방으로 생긴다.
	return SourceLocation + Forward * (AttackRange * 0.5f) + FVector::UpVector * HeightOffset;
}

FVector AGATA_AttackHitCheck::GetAttackBoxHalfExtent() const
{
	return FVector(AttackRange * 0.5f, AttackWidth * 0.5f, AttackHeight * 0.5f);
}
