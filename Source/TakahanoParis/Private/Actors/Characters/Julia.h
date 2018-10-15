// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Hero.h"
#include "Julia.generated.h"

/**
 *	@class AJulia
 *	@brief The CPP class behind the Julia Character
 */
UCLASS()
class AJulia : public AHero
{
	GENERATED_BODY()

public :
	// No need for constructor as there's nothing more than whats already in 
	//AJulia();


	/**
	 * @fn TryUseElectronic()
	 * @brief Julia can control certain electronic objects
	 * @param Target : An Actor that implements InteractInterface
	 * @return true if it succeed
	 */
	UFUNCTION()
		bool TryUseElectronic(AActor * Target);


	/* Tick function - called each frame*/
	void Tick(float DeltaSeconds) override;


	/**
	 * @fn GetLookedAtActor()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION()
		bool GetLookedAtActor(TArray<AActor*> &OutActors);
	
	
	
};
