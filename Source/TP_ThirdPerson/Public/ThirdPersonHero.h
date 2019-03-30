// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"
#include "ThirdPersonGameMode.h"
#include "ThirdPersonHero.generated.h"




/**
 *	ThirdPersonHero class - it handle all the ability and the more gameplay focused dynamics.
 *	All input , Camera and movement settings are done in @see AThirdPersonCharacter
 */
UCLASS(ClassGroup = (Character))
class TP_THIRDPERSON_API AThirdPersonHero : public AThirdPersonCharacter
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	AThirdPersonHero();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/**
	 *	@fn Use
	 *	@brief thw use action called by keyboard or gamepad action
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable", meta = (DisplayName = "Use"))
	void Use();



	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)// meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
		
	virtual void SetupCamera();

protected:

	/**
	 * @fn TryUse()
	 * @brief Julia can control certain electronic objects
	 * @param Target : An Actor that implements InteractInterface
	 * @return true if it succeed
	 */
	UFUNCTION()
		bool TryUse(AActor * Target);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// inherit via ILifeInterface
	virtual bool I_TakeDamage(const float& DamageAmount, AActor* Instigator) override;


public :

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "GetVisibleInteractables"))
	FORCEINLINE TArray<AActor *>  GetVisibleInteractableActors() const { return VisibleInteractableActors; }

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "GetAllInteractables"))
	FORCEINLINE TArray<AActor *>  GetAllInteractableActors() const { return InteractableActors; }

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "Get Closest Interactable"))
		AActor * GetClosestInteractableActor(float &Distance) const;


private :

	UPROPERTY()
		TArray<AActor *> InteractableActors;

	UFUNCTION()
		void SetInteractableActors();

	UPROPERTY()
		TArray<AActor *> VisibleInteractableActors;

	UFUNCTION()
		void SetVisibleInteractableActors();
};
