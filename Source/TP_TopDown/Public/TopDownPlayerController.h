// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

UCLASS()
class ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	virtual void PauseMenu();
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();




public :

	UFUNCTION(BlueprintPure, Category = "Gamepad")
		FORCEINLINE bool IsGamepadInput() const { return bIsUsingGamepad; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void CancelMoveToLocation();

private:

	UPROPERTY(Transient)
		mutable bool bIsUsingGamepad;




public :
	bool InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override; 
	bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Gamepad")
		mutable float LastGamepadInputTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gamepad")
		bool bResetGamepadDetectionAfterNoInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gamepad")
	float GamepadTimeout;

	/** Common logic needed in both `InputAxis()` and `InputKey()` */
	void UpdateGamepad(const bool &bGamepad) const;



private:
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void SetCursorLocation();

	/** A decal that projects to the cursor location. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UPROPERTY(Transient)
		mutable FVector CursorLocation;

};


