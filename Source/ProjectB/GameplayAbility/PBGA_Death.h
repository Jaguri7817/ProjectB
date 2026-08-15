// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PBGA_Death.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API UPBGA_Death : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPBGA_Death();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;



	// -------------------------------------
	//  AbilityTask Callback
	// -------------------------------------
private:
	UFUNCTION()
	void OnDeadMontageCompleted();
		
};
