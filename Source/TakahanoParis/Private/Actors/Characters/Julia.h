// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Hero.h"
#include "Julia.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHackElectronicDelegate, AActor * , _ActorToUse);


/**
 *	@class AJulia
 *	@brief The CPP class behind the Julia Character
 */
UCLASS()
class AJulia : public AHero
{
	GENERATED_BODY()

public :
	
	AJulia();

	/* Tick function - called each frame*/
	void Tick(float DeltaSeconds) override;


	/**
	 * @fn GetLookedAtElectronic()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION()
		bool GetLookedAtHackable(TArray<class AActor*> &OutActors);

	/**
	 * @fn GetLookedAtElectronic()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION(BlueprintCallable, Category ="Julia", meta = (DisplayName = "Get lokked at Electronic"))
		bool GetLookedAtHackable_BP(TArray<class AActor*> &OutActors) {return  GetLookedAtHackable(OutActors);}

	/**
	 *	@property HackDelegate
	 *	@brief Delegate that gets fired when you want to hack an object
	 */
	UPROPERTY()
		FHackElectronicDelegate HackDelegate;

protected:

	/**
	 *	@fn TryHack()
	 *	@brief Attempt to hack an actor ( must implement IHackInterface)
 	 *	@param target : the object you want to hack
	 *	@return true if target is hackable and hacking tried, false otherwise
	 *	@see IHackInterface
	 */
	UFUNCTION()
		bool TryHack(AActor * target)const ;

		/**
	 *	@fn TryHack_BP()
	 *	@brief Attempt to hack an actor ( must implement IHackInterface)
 	 *	@param target : the object you want to hack
	 *	@return true if target is hackable and hacking tried, false otherwise
	 *	@see IHackInterface
	 *	@note For blueprint
	 */
	UFUNCTION(BlueprintCallable, Category="Julia")
		bool TryHack_BP(AActor * target)const {return TryHack(target);}



private:
	/**
	 *	@fn Hack()
	 *	@brief hack an actor ( must implement IHackInterface) called by delegate function
 	 *	@param target : the object you want to hack
	 *	@see IHackInterface
	 */
	UFUNCTION()
		void Hack(AActor * target);

	
	
	
};
