// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "PBAnimNotify_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API UPBAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "GameplayEvent")
	FGameplayTag EventTag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	
};
