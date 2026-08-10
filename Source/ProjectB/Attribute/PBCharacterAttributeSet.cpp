// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PBCharacterAttributeSet.h"

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

}
