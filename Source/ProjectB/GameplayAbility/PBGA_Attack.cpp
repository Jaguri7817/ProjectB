// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PBGA_Attack.h"
#include "AbilitySystemComponent.h"

// Tasks
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// Animation
#include "Animation/AnimMontage.h"


UPBGA_Attack::UPBGA_Attack()
{
	// 인스턴스 재사용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPBGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 공격 상태 초기화
	CurrentCombo = 0;
	bNextComboInput = false;

	if (!ComboMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	StartCombo();
}

void UPBGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentCombo = 0;
	bNextComboInput = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPBGA_Attack::StartCombo()
{
	CurrentCombo = 1;
	bNextComboInput = false;

	// -------------------------------------
	//  1. ComboCheck GameplayEvent 대기
	// -------------------------------------
	if (ComboCheckEventTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* ComboCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboCheckEventTag, nullptr, false, true);
		ComboCheckTask->EventReceived.AddDynamic(this, &UPBGA_Attack::OnComboCheckEvent);

		ComboCheckTask->ReadyForActivation();
	}

	// -------------------------------------
	//  2. AttackHit GameplayEvent 대기
	// -------------------------------------
	if (AttackHitEventTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* AttackHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, AttackHitEventTag, nullptr, false, true);
		AttackHitTask->EventReceived.AddDynamic(this, &UPBGA_Attack::OnAttackHitEvent);

		AttackHitTask->ReadyForActivation();
	}

	// -------------------------------------
	//  3. 추가 공격 입력 대기
	// -------------------------------------
	WaitComboInput();

	// -------------------------------------
	//  4. Combo Montage 실행
	// -------------------------------------
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayComboAttack"), ComboMontage, 1.0f, FName("Attack1"));

	MontageTask->OnCompleted.AddDynamic(this, &UPBGA_Attack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UPBGA_Attack::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UPBGA_Attack::OnMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UPBGA_Attack::WaitComboInput()
{
	if (CurrentCombo >= MaxCombo)
	{
		return;
	}

	UAbilityTask_WaitInputPress* InputTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);

	InputTask->OnPress.AddDynamic(this, &UPBGA_Attack::OnComboInputPressed);
	InputTask->ReadyForActivation();
}

void UPBGA_Attack::ComboCheck()
{
	if (CurrentCombo >= MaxCombo)
	{
		return;
	}

	if (!bNextComboInput)
	{
		return;
	}

	bNextComboInput = false;
	++CurrentCombo;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (!ASC)
	{
		return;
	}

	const FName NextSection = GetComboSectionName();
	ASC->CurrentMontageJumpToSection(NextSection);

	WaitComboInput();
}

FName UPBGA_Attack::GetComboSectionName() const
{
	return FName(*FString::Printf(TEXT("Attack%d"), CurrentCombo));
}

void UPBGA_Attack::PerformAttackHitCheck(const FGameplayEventData& Payload)
{
	// -------------------------------------
	//  TODO: 공격판정 | Trace -> TargetData 생성 -> GameplayEffect 적용
	// -------------------------------------
}

void UPBGA_Attack::OnComboInputPressed(float TimeWited)
{
	bNextComboInput = true;
}

void UPBGA_Attack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPBGA_Attack::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPBGA_Attack::OnComboCheckEvent(FGameplayEventData Payload)
{
	ComboCheck();
}

void UPBGA_Attack::OnAttackHitEvent(FGameplayEventData Payload)
{
	PerformAttackHitCheck(Payload);
}
