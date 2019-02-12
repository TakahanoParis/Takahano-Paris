// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Gameplay/TPGameMode.h"
#include "TPHero.generated.h"




/**
 *	TPHero class - it handle all the ability and the more gameplay focused dynamics.
 *	All input , Camera and movement settings are done in @see ABaseCharacter
 */
UCLASS(ClassGroup = (Character))
class TP_THIRDPERSON_API ATPHero : public ABaseCharacter
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	ATPHero();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/**
	 *	@fn Use
	 *	@brief thw use action called by keyboard or gamepad action
	 */
	UFUNCTION()
	virtual void Use();

	/**
	 *	@fn Use_BP
	 *	@brief Use Blueprint Event
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable", meta = (DisplayName = "Use"))
		void Use_BP();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)// meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Camera")
		void SetupCamera(ECameraTypeEnum ViewType);

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


	FORCEINLINE TArray<AActor *>  GetVisibleInteractableActors() const { return VisibleInteractableActors; }
	FORCEINLINE TArray<AActor *>  GetAllInteractableActors() const { return InteractableActors; }

	UFUNCTION()
		AActor * GetClosestInteractableActor(float &Distance) const;

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "Get Closest Interactable"))
		AActor * GetClosestInteractableActor_BP(float &Distance) const { return GetClosestInteractableActor(Distance); }

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "GetVisibleInteractables"))
		TArray<AActor *>  GetVisibleInteractableActors_BP() const { return GetVisibleInteractableActors(); }

	UFUNCTION(BlueprintPure, Category = "Interactable", meta = (DisplayName = "GetAllInteractables"))
		TArray<AActor *>  GetAllInteractableActors_BP() const { return GetAllInteractableActors(); }

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
