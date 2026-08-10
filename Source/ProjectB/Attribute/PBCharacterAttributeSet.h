// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PBCharacterAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadDelegate);

/**
 * 
 */
UCLASS()
class PROJECTB_API UPBCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPBCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, Attack);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, MaxAttack);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, HP);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, MaxHP);
	ATTRIBUTE_ACCESSORS(UPBCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	//virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	// Ω∫≈» º≥∞Ë
	UPROPERTY(BlueprintReadOnly, Category="Stat", Meta= (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HP;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHP;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	uint8 bIsDead = false;

};
