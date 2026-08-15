// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PBGA_Death.h"
#include "PBGA_Death.h"
#include "AbilitySystemComponent.h"

// Ability Tag
#include "Tag/PBGameplayTag.h"

// Ability Tasks
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

// CharacterBase를 상속받은 모든 Character 클래스의 죽음 처리
#include "Character/PBCharacterBase.h"

// Character Movement Controll
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


UPBGA_Death::UPBGA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	FAbilityTriggerData TriggerData;

	TriggerData.TriggerTag = TAG_Event_Character_Death;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UPBGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("[GA_Death] Death Ability Activated"));

	if (!TriggerEventData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APBCharacterBase* Character = CastChecked<APBCharacterBase>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	ASC->AddLooseGameplayTag(TAG_State_Dead);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimMontage* DeadMontage = Character->GetDeadMontage();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DeadMontage"), DeadMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPBGA_Death::OnDeadMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UPBGA_Death::OnDeadMontageCompleted);

	MontageTask->ReadyForActivation();

}

void UPBGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPBGA_Death::OnDeadMontageCompleted()
{
	APBCharacterBase* Character = CastChecked<APBCharacterBase>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character))
	{
		return;
	}
	Character->Die();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);;
}
