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


	/** overriden from BaseCharacter */

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value) override;

	/** Called for side to side input */
	virtual void MoveRight(float Value) override;



	/**
	 *	@property HackDelegate
	 *	@brief Delegate that gets fired when you want to hack an object
	 */
	UPROPERTY(BlueprintAssignable, Category = "Events")
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
		bool TryHack(AActor * target);

		/**
	 *	@fn TryHack_BP()
	 *	@brief Attempt to hack an actor ( must implement IHackInterface)
 	 *	@param target : the object you want to hack
	 *	@return true if target is hackable and hacking tried, false otherwise
	 *	@see IHackInterface
	 *	@note For blueprint
	 */
	UFUNCTION(BlueprintCallable, Category="Julia", meta = (DisplayName = "Try Hack"))
		bool TryHack_BP(AActor * target) {return TryHack(target);}

	/**
	 *	@fn ReturnToCharacter()
	 *	@brief make sure we're using the camera from this actor
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_ReturnToCharacter();

	/**
	 *	@fn ReturnToCharacter_BP()
	 *	@brief make sure we're using the camera from this actor
	 *	@note For blueprint
	 */
	UFUNCTION(BlueprintCallable, Category = "Julia", meta = (DisplayName = "Return to Julia"))
		void ReturnToCharacter_BP() { Server_ReturnToCharacter(); }
		

private:
	/**
	 *	@fn Hack()
	 *	@brief hack an actor ( must implement IHackInterface) called by delegate function
 	 *	@param target : the object you want to hack
	 *	@see IHackInterface
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Hack(AActor * target);


	/**
	 *	@property bIsUsingObject
	 *	@brief boolean flag to know if another object is currently used
	 */
	UPROPERTY(Replicated)
		bool bIsUsingObject;


	/**
	 *	@property UsedActor
	 *	@brief pointer to the used actor
	 */
	UPROPERTY(Replicated)
		AActor * UsedActor;


protected:

	/**
	 *	@property Hackables
	 *	@brief Array containing all the currently visible Actors having the IHackInterface
	 *	@see IHackInterface
	 */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Julia|Hack", meta = (DisplayName = "LookedAtElectronics"))
		TArray< AActor *> Hackables;



	/**
	 * @fn GetLookedAtElectronic()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Julia")
		bool GetLookedAtHackable(TArray<class AActor*> &OutActors) const;

	/**
	 * @fn GetLookedAtElectronic()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION(BlueprintCallable, Category = "Julia", meta = (DisplayName = "Get looked at Electronic"))
		bool GetLookedAtHackable_BP(TArray<class AActor*> &OutActors) const { return  GetLookedAtHackable(OutActors); }
		

	
	
	
};
