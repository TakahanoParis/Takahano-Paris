// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "HackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHackInterface : public UInteractInterface
{
	GENERATED_BODY()
};

/**
 *	@class IHackInterface
 *	@brief Hack interface used by character Julia.
 *	@see Julia
 */
class IHackInterface : public IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

protected:
	/**
	 *	@fn I_Hack()
	 *	@brief function to Hack into an interact-able object
	 *	@param User : the user that hacks
	 */
	UFUNCTION()
		virtual  void I_Hack(AController * User) = 0;


public:
	/**
	 *	@fn I_Server_Hack()
	 *	@brief Hack the Actor in that implements this interface
	 *	@param User : the controller that want to use the UObject that implement this interface
	 *	@return true if it was activated. false otherwise
	 *	@note replicated, run on server
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void I_Server_Hack(class AController * User);
		virtual void I_Server_Hack_Implementation(class AController * User);
	
};
