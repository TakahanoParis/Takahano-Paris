 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Interfaces/CharacterTeamInterface.h"
#include "Actors/Interfaces/CharacterLifeInterface.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "Actors/Interfaces/PlayerCharacterInterface.h"
#include "ThirdPersonCharacter.generated.h"


//forward declaration
class UCustomSaveGame;


/**
* @Class AThirdPersonCharacter
* @brief The base class of all heroes and controller character of TakahanoParis.
*/
UCLASS(MinimalApi)
class AThirdPersonCharacter : public ACharacter, public ICharacterTeamInterface, public ICharacterLifeInterface, public ISaveableActorInterface, public IPlayerCharacterInterface
{
    GENERATED_BODY()


public:
	AThirdPersonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );
    void OnConstruction(const FTransform& Transform) override;

private:

    UPROPERTY(Replicated) //ReplicatedUsing = OnRep_IsReady)
    bool bIsReady;
protected :
	void I_GetReadyByRef(bool * IsReadyRef) override { IsReadyRef = &bIsReady; }

protected:
    /** Called for forwards/backward input */
    virtual void MoveForward(float Value);

    /** Called for side to side input */
	virtual void MoveRight(float Value);


protected:

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface


	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;


protected:
	/**
	 *	@fn CanRun
	 *	@return true if the character can run  
	 */
	UFUNCTION()
	virtual bool CanRun();


	/**
	*	@fn GetRunStaminaDrain
	*	@return the amount of used stamina
	*/
	UFUNCTION()
		virtual float GetRunStaminaDrain();


public :
	/**
	 *	@fn Run
	 *	@brief Apply the necessary changes to be running
	 */
	UFUNCTION()
		virtual void Run();


	/**
	*	@fn Run
	*	@brief Apply the necessary changes to be running
	*/
	UFUNCTION(BlueprintCallable, Category = "Movement", meta = (DisplayName = "Run"))
		virtual void Run_BP() { Run(); }


	/**
	 *	@fn StopRun
	 *	@brief Apply the necessary changes to be running
	 */
	UFUNCTION()
		virtual void StopRunning();


	/**
	*	@fn Run
	*	@brief Apply the necessary changes to be running
	*/
	UFUNCTION(BlueprintCallable, Category = "Movement", meta = (DisplayName = "Stop Running"))
		virtual void StopRunning_BP() { StopRunning(); }

private:
	TArray<AActor *> InteractableActors;

public:

	virtual TArray<AActor *> * I_GetAllInteractableArray()  override { return &InteractableActors; }


protected:
    bool SetCharacter() override;
};
