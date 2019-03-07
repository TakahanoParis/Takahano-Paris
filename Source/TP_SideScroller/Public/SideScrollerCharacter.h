// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Interfaces/CharacterLifeInterface.h"
#include "Actors/Interfaces/CharacterTeamInterface.h"
#include "Actors/Interfaces/PlayerCharacterInterface.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "SideScrollerCharacter.generated.h"

UCLASS(config=Game)
class ASideScrollerCharacter : public ACharacter, public ICharacterTeamInterface, public ICharacterLifeInterface, public ISaveableActorInterface, public IPlayerCharacterInterface
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	/** Called for side to side input */
	virtual void MoveRight(float Val);

	/** Called for side to side input */
	virtual void MoveForward(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	ASideScrollerCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_bCanJump, Category = Character)
		bool bCanJump;

private :
	UFUNCTION()
		void OnRep_bCanJump();


private:

	UPROPERTY(Replicated) //ReplicatedUsing = OnRep_IsReady)
		bool bIsReady;

protected:
	void I_GetReadyByRef(bool * IsReadyRef) override { IsReadyRef = &bIsReady; }
	
	
	// Climbable interaction ----------------------------------------
private:

	UPROPERTY(Transient, Replicated)
		bool bCanClimb;

	UPROPERTY(Transient, Replicated)
		AActor * ClimbableActor;

public:
	UFUNCTION(BlueprintPure, Category = "Climb")
		bool GetCanClimb() const { return bCanClimb; }

	/** Allow the character to climb by calling the function on a valid ClimbableObject */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Climb")
		void SetCanClimb(bool bNewCanClimb, AActor* NewClimbableActor);

protected:

	/** Used for Displaying a message to the player about the fact he can climb something */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnCanClimb(const AActor * Climbable);

	/**
	*	Used for Doing the actual climbing
	*	@return false if it failed to climb
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool Climb(const AActor * Climbable);

};
