// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LifeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalApi, meta = (CannotImplementInterfaceInBlueprint))
class ULifeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	@class ILifeInterface
 *	@brief class allow all 
 */
class TAKAHANOPARIS_API ILifeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	 *	@fn I_GetLifePoints()
	 *	@brief Get the life point of the actor
	 *	@return life points
	 *	@note the variable is ideally stored in PlayerState and/or replicated
	 */
	UFUNCTION(BlueprintCallable, Category = "Life", meta = (DisplayName = "Get Life Points"))
		virtual float I_GetLifePoints() const = 0;

	/**
	 *	@fn I_takeDamage()
	 *	@brief Receive damage
	 *	@param DamageAmount : Damage you want to inflight
	 *	@param Instigator  : The actor responsible for the damage
	 *	@return true if damage were taken 
	 */
	UFUNCTION(BlueprintCallable, Category = "Life", meta = (DisplayName = "Take damage"))
		virtual bool I_TakeDamage(const float &DamageAmount, class AActor * Instigator) = 0;



	
};
