// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Interfaces/CharacterLifeInterface.h"
#include "Actors/Interfaces/CharacterTeamInterface.h"
#include "Actors/Interfaces/PlayerCharacterInterface.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "SideScrollerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartClimbAnimDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClimbAnimFinishedDelegate, const AActor*, ClimbTarget);

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

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Jump interaction			-----------------------------------------------------------------


	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_bCanJump, Category = Character)
		bool bCanJump;

private :
	UFUNCTION()
		void OnRep_bCanJump();

	
	// Climbable interaction	-----------------------------------------------------------------
private:

	UPROPERTY(Transient, Replicated)
		bool bCanClimb;


	/** Allow the character to climb by calling the function on a valid ClimbableObject */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetCanClimb(bool bNewCanClimb, AActor* NewClimbableActor);

public:
	UFUNCTION(BlueprintPure, Category = "Climb")
		bool GetCanClimb() const { return bCanClimb; }


	/** Allow the character to climb by calling the function on a valid ClimbableObject */
	UFUNCTION(BlueprintCallable, Category = "Climb")
		void SetCanClimb(bool bNewCanClimb, AActor* NewClimbableActor);

	UFUNCTION(BlueprintPure, Category = "Climb")
		AActor * GetClimbableActor() const { return ClimbableActor; };


protected:

	UPROPERTY(Transient, Replicated)
		AActor * ClimbableActor;

	/** Used for Displaying a message to the player about the fact he can climb something */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnCanClimb(AActor * Climbable);

	/**
	*	Used for Doing the actual climbing
	*	@return false if it failed to climb
	*/
	UFUNCTION()
		void Climb();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void FinishClimb(const AActor* ClimbTarget);

public :
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, VisibleAnywhere)
		FOnClimbAnimFinishedDelegate OnClimbAnimFinished;


	UFUNCTION(BlueprintCallable)
		void BroadCastFinishClimb();

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, VisibleAnywhere)
		FOnStartClimbAnimDelegate StartClimbAnim;

	// Limit Axis Movement		-----------------------------------------------------------------
private :

	UPROPERTY(Replicated, Transient)
		bool bCanMoveY;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetCanMoveY(bool bNewCanMoveY);

public :
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetCanMoveY(bool bNewCanMoveY);


protected:
	bool SetCharacter() override;
};
