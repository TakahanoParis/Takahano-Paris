// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Hero.generated.h"



/**
 *	Hero class - it handle all the ability and the more gameplay focused dynamics.
 *	All input , Camera and movement settings are done in @see ABaseCharacter
 */
UCLASS(ClassGroup = (Character))
class TAKAHANOPARIS_API AHero : public ABaseCharacter
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	AHero();

	void BeginPlay() override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)// meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UFUNCTION()
		virtual bool GetLookedAtInteractables(TArray<AActor* > &Interactables) const;

	UFUNCTION()
		virtual void Use();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable", meta = (DisplayName = "Use"))
		void Use_BP();


private:

	/**
	 *	@property InteractableActors
	 *	@brief	all interactables actors found at begin Play. Improves logic and speed to look here
	 */
	UPROPERTY(Replicated)
		TArray<AActor*>	InteractableActors;



	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetInteractables();



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

protected:
	virtual bool CanRun() override;
public:
	virtual void Run() override;
		
};
