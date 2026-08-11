// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/GasInputActionDataAsset.h"

const UInputAction* UGasInputActionDataAsset::FindAbilityInputAction(EAbilityInputID InputID) const
{
    for (const auto& AbilityInput : AbilityInputActions)
    {
        if (AbilityInput.InputID == InputID)
        {
            return AbilityInput.InputAction;
        }
    }

    return nullptr;
}
