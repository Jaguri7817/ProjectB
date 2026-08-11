// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Input/AbilityInputID.h"
#include "GasInputActionDataAsset.generated.h"


/**
 * GAS 입력 하나를 표현하는 데이터
 *
 * InputAction : Enhanced Input에서 발생하는 입력
 * InputID     : 해당 입력과 연결할 GAS InputID
 */
class UInputAction;

USTRUCT(BlueprintType)
struct FPBAbilityInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EAbilityInputID InputID = EAbilityInputID::None;
};

UCLASS()
class PROJECTB_API UGasInputActionDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Input")
	TArray<FPBAbilityInputAction> AbilityInputActions;
	
public:
	const UInputAction* FindAbilityInputAction(EAbilityInputID InputID) const;
};
