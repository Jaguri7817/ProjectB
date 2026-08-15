// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PBCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/PBGameplayTag.h"

UPBCharacterAttributeSet::UPBCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	Attack(30.0f),
	MaxAttack(100.0f),
	MaxHP(100.0f),
	Damage(0.0f)
{
	InitHP(GetMaxHP());
}

void UPBCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHP());
	}
}

void UPBCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

}

void UPBCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] HP: %.1f / %.1f"), *GetNameSafe(GetOwningActor()), GetHP(), GetMaxHP());
	}

	if (GetHP() <= 0.0f && !Data.Target.HasMatchingGameplayTag(TAG_State_Dead))
	{
		UE_LOG(LogTemp, Log, TEXT("[Post Effect Execute] Character Death Processing"));

		AActor* TargetActor = Data.Target.GetAvatarActor();

		
		FGameplayEventData EventData;
		EventData.EventTag = TAG_Event_Character_Death;

		Data.Target.HandleGameplayEvent(TAG_Event_Character_Death, &EventData);
		
		
	}
	
}