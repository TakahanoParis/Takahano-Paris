// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"


class AActor;

/**
 * Uinterface class
 * This class does not need to be modified.
 */
UINTERFACE(MinimalAPI, meta =(CannotImplementInterfaceInBlueprint))
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IPlayerCharacterInterface
 * Player Character interface. add all the necessary stuff requiered for all game modes
 */
class TAKAHANOPARIS_API IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Ready Setting ----------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Character Interface|Gameplay")
		virtual void I_SetReady(bool NewReady);

	UFUNCTION(BlueprintCallable, Category = "Character Interface|Gameplay")
		virtual bool I_GetIsReady();

private :
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void I_Server_IsReady(bool ServerIsReady);
protected:
		virtual void I_Server_IsReady_Implementation(bool ServerIsReady);

protected :
	UFUNCTION()
		virtual void I_OnRep_IsReady();

	/** the most important to imlement, the rest should work them */
		virtual void I_GetReadyByRef(bool * IsReadyRef) = 0;

	// Jump Setting ------------------------------------------------------------
		UFUNCTION(BlueprintCallable, Category = "Character Interface|Jump")
		virtual void I_SetCanJump(bool NewCanJump);

private:
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void I_Server_SetCanJump(bool NewCanJump);
protected:
		virtual void I_Server_SetCanJump_Implementation(bool NewCanJump);




};
