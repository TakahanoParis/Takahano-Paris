// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ObjectMacros.h"
#include "InteractInterface.generated.h"


UENUM(BlueprintType)
enum class EInteractableState : uint8
{
	ISE_Off UMETA(DisplayName = "Off"),
	ISE_Locked UMETA(DisplayName = "Locked"),
	ISE_Used UMETA(DisplayName = "Used"),
	ISE_Useable UMETA(DisplayName="Usable")
};

// This class does not need to be modified.
UINTERFACE(MinimalApi, meta = (CannotImplementInterfaceInBlueprint))
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IInteractInterface
 * Interaction interface. allows to ask actors to be used.
 * This class is virtual and needs to be implemented to be in a working class
 */
class TAKAHANOPARIS_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 *	@brief I_GetInteractState function
	 *	Return a EInteractableState describing the object interaction possibilities.
	 */
	UFUNCTION()
		virtual EInteractableState I_GetInteractState() = 0;

	/**
		* @brief I_GetInteractState_BP function.
		* @return a EInteractableState describing the object interaction possibilities.
		* @param NewTeam the index of the team you want to be in
		* @note  for Blueprint, no need to implement it in your classes
		*/
	UFUNCTION(BlueprintCallable,  meta = (DisplayName = "Get Interactable State"))
		virtual EInteractableState I_GetInteractState_BP() { return I_GetInteractState(); }

	/**
	 *	@fn I_Use function
	 *	@return true if it was activated.
	 */
	UFUNCTION()
		virtual bool I_Use(class AController * User ) = 0;

	/** 
	 *	@fn I_Use
	 *	@brief Return true if it was activated.
	 *	@note  for Blueprint, no need to implement it in your classes
	 */
	UFUNCTION(BlueprintCallable,  meta = (DisplayName = "Use"))
		virtual bool I_Use_BP(class AController * User) { return I_Use(User); }


	
};
