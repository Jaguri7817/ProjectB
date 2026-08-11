#pragma once

#include "CoreMinimal.h"
#include "AbilityInputID.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None = 0,

	Attack,
	Roll,
	Skill1,
	Skill2
};