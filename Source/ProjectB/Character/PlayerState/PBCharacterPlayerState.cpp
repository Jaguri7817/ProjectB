// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerState/PBCharacterPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PBCharacterAttributeSet.h"

APBCharacterPlayerState::APBCharacterPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UPBCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

UAbilitySystemComponent* APBCharacterPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
