// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PBCharacterBase.generated.h"

UCLASS()
class PROJECTB_API APBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APBCharacterBase();

public:
	void Die();
	inline UAnimMontage* GetDeadMontage() const {  return DeadMontage;	};

protected:
	virtual void BeginPlay() override;

	virtual void SetDead();

	virtual void SpawnCharacter();

	// -------------------------------------
	//  Montge
	// -------------------------------------
	UPROPERTY(EditAnywhere, Category = "Dead")
	TObjectPtr<UAnimMontage> DeadMontage;

	
};
