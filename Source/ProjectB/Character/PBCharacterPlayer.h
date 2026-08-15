// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PBCharacterBase.h"
#include "AbilitySystemInterface.h"
//#include "Abilities/GameplayAbilityTypes.h"
#include "Input/AbilityInputID.h"
#include "PBCharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UGasInputActionDataAsset;
class UEnhancedInputcomponent;

/**
 * 
 */
UCLASS()
class PROJECTB_API APBCharacterPlayer : public APBCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
public:
	APBCharacterPlayer();
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	void SetupGASPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

	void AbilityInputPressed(EAbilityInputID InputID);
	void AbilityInputReleased(EAbilityInputID InputID);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPriavteAccess = "true"))
	TObjectPtr<UGasInputActionDataAsset> InputConfig;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	virtual void OnRep_PlayerState() override;
	void InitAbilitySystem();

	UPROPERTY(EditAnywhere, Category = "GAS|Ability")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;

	UPROPERTY(EditAnywhere, Category = "GAS|Input Ability")
	TMap<EAbilityInputID, TSubclassOf<class UGameplayAbility>> InputAbilities;

private:
	virtual void SetDead() override;
	virtual void SpawnCharacter() override;
};
