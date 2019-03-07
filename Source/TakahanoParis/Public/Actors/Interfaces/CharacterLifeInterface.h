// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LifeInterface.h"
#include "CharacterLifeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalApi, meta = (CannotImplementInterfaceInBlueprint))
class UCharacterLifeInterface : public ULifeInterface
{
	GENERATED_BODY()
};

/**
 *	@class ILifeInterface
 *	@brief class allow all 
 */
class TAKAHANOPARIS_API ICharacterLifeInterface: public ILifeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	

		virtual float I_GetLifePoints() const override;

		virtual bool I_TakeDamage(const float &DamageAmount, class AActor * Instigator) override;

};
