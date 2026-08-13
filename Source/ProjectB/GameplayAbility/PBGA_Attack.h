// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PBGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API UPBGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPBGA_Attack();


public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	// -------------------------------------
	//  Montge
	// -------------------------------------
	UPROPERTY(EditAnywhere, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboMontage;


	// -------------------------------------
	//  Combo
	// -------------------------------------
	int32 CurrentCombo = 0;
	static constexpr int32 MaxCombo = 3;

	bool bNextComboInput = false;


	// -------------------------------------
	//  Gameplay Tag
	// -------------------------------------
	UPROPERTY(EditAnywhere, Category = "Combo|Event")
	FGameplayTag ComboCheckEventTag;

	UPROPERTY(EditAnywhere, Category = "Combo|EventTag")
	FGameplayTag AttackHitEventTag;

private:
	void StartCombo();
	
	void WaitComboInput();

	void ComboCheck();

	FName GetComboSectionName() const;

	void PerformAttackHitCheck(const FGameplayEventData& Payload);

	UPROPERTY(EditAnywhere, Category = "Attack|Targeting")
	TSubclassOf<class AGameplayAbilityTargetActor> TargetActorClass;


private:
	// -------------------------------------
	//  AbilityTask Callback
	// -------------------------------------
	UFUNCTION()
	void OnComboInputPressed(float TimeWited);
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnComboCheckEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnAttackHitEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
};

