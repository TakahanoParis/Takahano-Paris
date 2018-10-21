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
	UFUNCTION()
		float I_GetLifePoints() const = 0;


	/**
	 *	@fn I_GetLifePoints_BP()
	 *	@brief Get the life point of the actor
	 *	@return life points
	 *	@note the variable is ideally stored in PlayerState and/or replicated
	 *	@note For Blueprints
	 */
	UFUNCTION()
		float I_GetLifePoints_BP() const {return I_GetLifePoints();}


	/**
	 *	@fn I_takeDamage()
	 *	@brief Receive damage
	 *	@param DamageAmount : Damage you want to inflight
	 *	@param Instigator  : The actor responsible for the damage
	 *	@return true if damage were taken 
	 */
	UFUNCTION()
		bool I_takeDamage(float DamageAmount, class AActor * Instigator) = 0;

	/**
	 *	@fn I_takeDamage_BP()
	 *	@brief Receive damage
	 *	@param DamageAmount : Damage you want to inflight
	 *	@param Instigator  : The actor responsible for the damage
	 *	@return true if damage were taken
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category ="Life", meta= (DisplayName = "Take damage") )
		bool I_takeDamage_BP(float DamageAmount, class AActor * Instigator){return I_takeDamage(DamageAmount, Instigator);}



	
};
