// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonHero.h"
#include "ThirdPersonJulia.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHackElectronicDelegate, AActor * , _ActorToUse);


/**
 *	@class AThirdPersonJulia
 *	@brief The CPP class behind the ThirdPersonJulia Character
 */
UCLASS()
class AThirdPersonJulia : public AThirdPersonHero
{
	GENERATED_BODY()

public :
	
	AThirdPersonJulia();


	/* Tick function - called each frame*/
	void Tick(float DeltaSeconds) override;


	/** overriden from BaseCharacter */

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value) override;

	/** Called for side to side input */
	virtual void MoveRight(float Value) override;

	/** Used to save the avaible actors */
	void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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
	UFUNCTION(BlueprintCallable, Category="ThirdPersonJulia", meta = (DisplayName = "Try Hack"))
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
	UFUNCTION(BlueprintCallable, Category = "ThirdPersonJulia", meta = (DisplayName = "Return to ThirdPersonJulia"))
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


	UFUNCTION(BlueprintPure)
		bool GetIsUsingObject() const { return bIsUsingObject; }


	UFUNCTION(BlueprintPure, Category = "Interactable|Hackables", meta = (DisplayName = "GetVisibleHackables"))
	FORCEINLINE TArray< AActor *>  GetVisibleHackables() const { return VisibleHackables; }

	UFUNCTION(BlueprintPure, Category = "Interactable|Hackables")
		AActor * GetHackTarget() const;


	/**
	 * @fn GetLookedAtElectronic()
	 * @brief Find all actors in center of screen provided by the player controller
	 * @param OutActors : the array that will be filled with the actors given by the player controller
	 * @return true if found something, false otherwise.
	 * @see ACustomPlayerController
	 */
	UFUNCTION(BlueprintCallable, Category = "ThirdPersonJulia", meta = (DisplayName = "Get looked at Hackables"))
		bool SetVisibleHackables();


	
		
private:

	/**
	 *	@property HackableActors
	 *	@brief	all hackable actors found at begin Play. Improves logic and speed to look here
	 */
	UPROPERTY(Replicated)
	TArray<AActor*>	Hackables;

	/**
 *	@property Hackables
 *	@brief Array containing all the currently visible Actors having the IHackInterface
 *	@see IHackInterface
 */
	UPROPERTY(Replicated, Transient)
		TArray< AActor *> VisibleHackables;

	UPROPERTY(Transient)
		uint16 TargetIndex;

	UPROPERTY(Replicated, Transient)
		AActor * Target;

public :
	UFUNCTION()
		void IncrementTarget();

	UFUNCTION()
		void DecrementTarget();

protected:
	bool SetCharacter() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable|Hackables")
		int MaxSelections;








};
