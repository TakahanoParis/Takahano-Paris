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
UINTERFACE(MinimalApi)
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
		virtual EInteractableState I_GetInteractState() const;

	/**
	 *	@fn I_Use function
	 *	@return true if it was activated.
	 */
	UFUNCTION()
		virtual bool I_Use(class AController * User );



	/** 
	 *	@fn I_Use
	 *	@brief Use the Actor in that implements this interface
	 *	@param User : the controller that want to use the UObject that implement this interface
	 *	@return true if it was activated. false otherwise
	 *	@note  for Blueprint, no need to implement it in your classes
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interact", meta = (DisplayName = "Use"))
		void I_Use_BP(class AController * UserController);

	/**
	 *	@fn I_Server_Use()
	 *	@brief Use the Actor in that implements this interface
	 *	@param User : the controller that want to use the UObject that implement this interface
	 *	@return true if it was activated. false otherwise
	 *	@note replicated, run on server
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void I_Server_Use(class AController * User);
		virtual void I_Server_Use_Implementation(class AController * User);


	
};
