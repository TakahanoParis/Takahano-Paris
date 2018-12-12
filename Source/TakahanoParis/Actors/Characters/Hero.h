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

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)// meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


protected :

    virtual void OnConstruction(const FTransform & Transform) override;


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
