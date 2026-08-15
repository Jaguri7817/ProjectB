// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PBCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Character/PlayerState/PBCharacterPlayerState.h"
#include "Attribute/PBCharacterAttributeSet.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/GasInputActionDataAsset.h"
#include "InputAction.h"
#include "Tag/PBGameplayTag.h"

APBCharacterPlayer::APBCharacterPlayer()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerFollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

}

UAbilitySystemComponent* APBCharacterPlayer::GetAbilitySystemComponent() const
{
    return ASC;
}

void APBCharacterPlayer::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

	InitAbilitySystem();

	const UPBCharacterAttributeSet* PBCharacterAS = ASC->GetSet<UPBCharacterAttributeSet>();
	if (PBCharacterAS)
	{
		// 어트리뷰트 델리게이트 세팅

	}

	for (const auto& Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability);
		ASC->GiveAbility(AbilitySpec);
	}

	for (const auto& InputAbility : InputAbilities)
	{
		FGameplayAbilitySpec InputSpec(InputAbility.Value);

		InputSpec.InputID = static_cast<int32>(InputAbility.Key);

		ASC->GiveAbility(InputSpec);
	}
}

void APBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APBCharacterPlayer::Move);
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APBCharacterPlayer::Look);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APBCharacterPlayer::Look);

		// Gas Input Setting
		SetupGASPlayerInputComponent(EnhancedInputComponent);
    }    
}

void APBCharacterPlayer::SetupGASPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	check(InputConfig);

	// GAS 시스템 전용 입력 바인드
	for (const auto& AbilityInput : InputConfig->AbilityInputActions)
	{
		if (!IsValid(AbilityInput.InputAction))
		{
			continue;
		}
		if (AbilityInput.InputID == EAbilityInputID::None)
		{
			continue;
		}

		EnhancedInputComponent->BindAction(AbilityInput.InputAction, ETriggerEvent::Started, this, &APBCharacterPlayer::AbilityInputPressed, AbilityInput.InputID);
		EnhancedInputComponent->BindAction(AbilityInput.InputAction, ETriggerEvent::Completed, this, &APBCharacterPlayer::AbilityInputReleased, AbilityInput.InputID);
	}
}

void APBCharacterPlayer::AbilityInputPressed(EAbilityInputID InputID)
{
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputID));

	if (!Spec)
	{
		return;
	}

	Spec->InputPressed = true;

	if (Spec->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Active Ability Second Input"));
		//ASC->AbilitySpecInputPressed(*Spec);				// GAS 입력 처리 파이프라인의 한 단계만 직접 호출
		ASC->PressInputID(static_cast<int32>(InputID));		// ASC에게 InputID만 준 뒤, ASC가 알아서 입력 처리하도록
	}
	else
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

void APBCharacterPlayer::AbilityInputReleased(EAbilityInputID InputID)
{
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputID));

	if (!Spec)
	{
		return;
	}

	Spec->InputPressed = false;

	if (Spec->IsActive())
	{
		//ASC->AbilitySpecInputReleased(*Spec);
		ASC->ReleaseInputID(static_cast<int32>(InputID));
	}
}

void APBCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystem();
}

void APBCharacterPlayer::InitAbilitySystem()
{
	APBCharacterPlayerState* PBCharacterPS = GetPlayerState<APBCharacterPlayerState>();
	if (PBCharacterPS)
	{
		ASC = PBCharacterPS->GetAbilitySystemComponent();

		if (ASC)
		{
			ASC->InitAbilityActorInfo(PBCharacterPS, this);
		}
	}
}

void APBCharacterPlayer::SetDead()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	
}

void APBCharacterPlayer::SpawnCharacter()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	ASC->RemoveLooseGameplayTag(TAG_State_Dead);

	const UPBCharacterAttributeSet* AttributeSet = ASC->GetSet<UPBCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return;
	}

	// 어트리뷰트셋에 직접 접근하여 HP값을 수정하는게 껄끄럽긴 하지만
	// 이것마저 GA, GE를 써서 만들면 코드 양이 더 많아지기 때문에 
	// 트레이드 오프
	ASC->SetNumericAttributeBase(UPBCharacterAttributeSet::GetHPAttribute(), AttributeSet->GetMaxHP());
	
}

void APBCharacterPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APBCharacterPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APBCharacterPlayer::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void APBCharacterPlayer::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APBCharacterPlayer::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void APBCharacterPlayer::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
