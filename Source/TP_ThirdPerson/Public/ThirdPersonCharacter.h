 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Interfaces/CharacterTeamInterface.h"
#include "Actors/Interfaces/CharacterLifeInterface.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "Actors/Interfaces/PlayerCharacterInterface.h"
#include "ThirdPersonCharacter.generated.h"


 class AHackCameraActor;
 class AFloorActor;
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


protected:
    /** Called for forwards/backward input */
    virtual void MoveForward(float Value);

    /** Called for side to side input */
	virtual void MoveRight(float Value);

	/** Called for X Rotation input on Gamepad */
	virtual void TurnAtRate(float Rate);

	/** Called for Y Rotation input on Gamepad */
	virtual void LookUpAtRate(float Rate);

	/** Called for X Rotation input */
	virtual void Turn(float Value);

	/** Called for Y Rotation input */
	virtual void LookUp(float Value);

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Camera")
    float BaseTurnRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BaseLookUpRate;

protected:

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface


	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;



protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)// meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
  


    /**
	*	@fn Use
	*	@brief the use action called by keyboard or gamepad action
	*/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
    virtual void Use();

	/**
	*	@fn Cancel()
	*	@brief the cancel use action called by keyboard or gamepad action
	*/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void Cancel();


	/**
	*	@fn Cancel()
	*	@brief the cancel use action called by keyboard or gamepad action
	*/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void IncrementTarget();

	/**
	*	@fn Cancel()
	*	@brief the cancel use action called by keyboard or gamepad action
	*/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void DecrementTarget();


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
	UFUNCTION(BlueprintCallable, Category = "Movement", meta = (DisplayName = "Run"))
		virtual void Run();

	/**
	 *	@fn StopRun
	 *	@brief Apply the necessary changes to be running
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement", meta = (DisplayName = "Stop Running"))
		virtual void StopRunning();



protected:
    bool SetCharacter() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor")
		uint8 FloorIndex;

public:
	UFUNCTION(BlueprintPure, Category = "Floor")
		FORCEINLINE uint8 GetFloorIndex() const	{return FloorIndex;	}


	friend AFloorActor;
	friend AHackCameraActor;
};
