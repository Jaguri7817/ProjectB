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

protected:
	virtual void BeginPlay() override;


	void SetDead();

	void SpawnCharacter();


	
};
