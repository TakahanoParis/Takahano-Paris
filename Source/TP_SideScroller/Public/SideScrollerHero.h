// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerCharacter.h"
#include "SideScrollerHero.generated.h"




/**
 *	Hero class - it handle all the ability and the more gameplay focused dynamics.
 *	All input , Camera and movement settings are done in @see ABaseCharacter
 */
UCLASS(ClassGroup = (Character))
class TP_SIDESCROLLER_API ASideScrollerHero : public ASideScrollerCharacter
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	ASideScrollerHero();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Use System -----------------------------------------------------------------
public:
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

	/**
	 * @fn TryUse()
	 * @brief Julia can control certain electronic objects
	 * @param Target : An Actor that implements InteractInterface
	 * @return true if it succeed
	 */
	UFUNCTION()
		bool TryUse(AActor * Target);

public:
	

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
