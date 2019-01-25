 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Interfaces/TeamInterface.h"
#include "Actors/Interfaces/LifeInterface.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "BaseCharacter.generated.h"


//forward declaration
class UCustomSaveGame;


/**
* @Class ABaseCharacter
* @brief The base class of all heroes and controller character of TakahanoParis.
*/
UCLASS(ClassGroup = (Character), config=Game)

class TAKAHANOPARIS_API ABaseCharacter : public ACharacter, public ITeamInterface, public ILifeInterface, public ISaveableActorInterface
{
    GENERATED_BODY()


public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void SetReady(bool NewReady);

    UFUNCTION(BlueprintCallable, Category ="Gameplay")
    bool GetIsReady() const;

private:

    UPROPERTY(Replicated) //ReplicatedUsing = OnRep_IsReady)
    bool bIsReady;

    UFUNCTION()
    void OnRep_IsReady();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_IsReady(bool ServerIsReady);

protected:
    /** Called for forwards/backward input */
    virtual void MoveForward(float Value);

    /** Called for side to side input */
	virtual void MoveRight(float Value);


	/**
	 *	@brief Ability Function 
	 *	Will try to throw the ability if it exist
	 *	@return false if it did nothing
	 */
	UFUNCTION()
	virtual bool Ability(const uint8 &Number);




protected:

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface



	// Inherited via ITeamInterface
	virtual void I_SetTeam(FTeam NewTeam) override;

	virtual FTeam I_GetTeam() const override;


	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

public:
	virtual float I_GetLifePoints() const override;
	virtual bool I_TakeDamage(const float& DamageAmount, AActor* Instigator) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Character)
		bool bCanJump;

private:
	virtual bool CanJumpInternal_Implementation() const override;


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



};
