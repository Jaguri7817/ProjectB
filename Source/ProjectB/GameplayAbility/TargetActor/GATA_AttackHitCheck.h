// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATA_AttackHitCheck.generated.h"

/**
 *  캐릭터 기준 전방 공격에 대한 타겟 액터 클래스이다.
 * 
 *  공격 판정은 Box에 오버랩된 액터들을 대상으로 한다.
 * 
 *  Box 범위는 하드코딩으로 임시 지정했다. (테스트 후 캐릭터 어트리뷰트 기반으로 설정할 것임)
 * 
 *  마찬가지로 최종 공격 대상으로 인정할 클래스를 임시로 Pawn으로 해두었다.
 * 
 */
UCLASS()
class PROJECTB_API AGATA_AttackHitCheck : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AGATA_AttackHitCheck();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

private:
	FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool IsValidTarget(const AActor* Candidate) const;

	FVector GetAttackBoxCenter() const;

	FVector GetAttackBoxHalfExtent() const;


private:
	UPROPERTY(EditAnywhere, Category = "Targeting|Range", meta = (ClampMin = "0.0"))
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Targeting|Range", meta = (ClampMin = "0.0"))
	float AttackWidth = 160.0f;

	UPROPERTY(EditAnywhere, Category = "Targeting|Range", meta = (ClampMin = "0.0"))
	float AttackHeight = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Targeting|Range")
	float HeightOffset = 60.0f;
	
	UPROPERTY(EditAnywhere, Category = "Targeting|Collision")
	TEnumAsByte<ECollisionChannel> TargetObjectType = ECC_Pawn;
	
	UPROPERTY(EditAnywhere, Category = "Targeting|Filter")
	TSubclassOf<AActor> RequiredTargetClass;
};
