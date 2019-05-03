// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonHero.h"
#include "ThirdPersonJulia.generated.h"


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

	virtual void SetUsableActors() override;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Use() override;
	virtual void Cancel() override;

protected:

	bool SetCharacter() override;

	UPROPERTY()
		bool bUsingCamera;

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
	



};
