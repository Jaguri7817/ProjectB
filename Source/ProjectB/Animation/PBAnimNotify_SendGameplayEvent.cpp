// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PBAnimNotify_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"


void UPBAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp || !EventTag.IsValid())
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();

	if (!Owner)
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = Owner;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, Payload);
}
