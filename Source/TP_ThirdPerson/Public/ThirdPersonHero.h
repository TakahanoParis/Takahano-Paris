// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"
#include "ThirdPersonGameMode.h"
#include "ThirdPersonHero.generated.h"

class AActivableActor;

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

	virtual void Use() override;
	virtual void IncrementTarget() override { TargetIndex < uint32(UsableActors.Num()) ? TargetIndex++ : TargetIndex = 0; }
	virtual void DecrementTarget() override { TargetIndex > uint32(0) ? TargetIndex-- : UsableActors.Num(); }

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// inherit via ILifeInterface
	virtual bool I_TakeDamage(const float& DamageAmount, AActor* Instigator) override;


private :

	UPROPERTY(Replicated)
		TArray<AActivableActor *> InteractableActors;
	

	UFUNCTION()
		void SetInteractableActors();

	UPROPERTY()
		TArray<AActivableActor *> VisibleInteractableActors;

	UFUNCTION()
		void SetVisibleInteractableActors();

protected:
	UPROPERTY(Replicated)
	bool bIsUsingObject;


	UPROPERTY(Replicated)
		uint32 TargetIndex;

	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Use")
	TArray<AActivableActor*> UsableActors;


protected :
	UFUNCTION()
	virtual void SetUsableActors();

public:

	UFUNCTION(BlueprintPure, Category = "Use")
		FORCEINLINE TArray<AActivableActor*> GetUsableActors() const { return UsableActors; }


	UFUNCTION(BlueprintPure, Category = "Use")
		int GetTargetIndex() const { return TargetIndex; }



protected:
	void MoveForward(float Value) override;
	void MoveRight(float Value) override;
};
