// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PBCharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API APBCharacterPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	
public:
	APBCharacterPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UPBCharacterAttributeSet> AttributeSet;
	
};
