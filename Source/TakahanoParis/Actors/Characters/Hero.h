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

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

protected:

    /**
     *      List of abilities
     */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Ability", meta = (DisplayName = "Abilities"))
	    TArray<class UAbility*> Abilities;

	/**
	 *      List of ability classes
	 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (DisplayName = "Ability List"))
        TArray< TSubclassOf<class UAbility>> AbilityClasses;




protected :

    virtual void OnConstruction(const FTransform & Transform) override;


private:

//	FORCEINLINE virtual void Attack()			override { AraAbility(); }

public:
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	
};
