// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterInterface.h"
#include "TopDownCharacter.generated.h"

UCLASS(Abstract)
class ATopDownCharacter : public ACharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	ATopDownCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) override;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	class UDecalComponent* CursorToWorld;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		class UMaterial * DecalMaterial;


protected:
	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

public:

	/**
	*	@fn Use
	*	@brief thw use action called by keyboard or gamepad action
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable", meta = (DisplayName = "Use"))
		void Use();

	/**
	 * @brief shows the cursor 
	 */
	UPROPERTY()
		bool bMoveWithCursor = true;

	UFUNCTION()
		void MoveToCursor();

private:
	bool bIsReady;

	mutable TArray<AActor*> Interactables;

protected:

	void I_GetReadyByRef(bool* IsReadyRef) override { IsReadyRef =  &bIsReady; }
	TArray<AActor*>* I_GetAllInteractableArray() override { return &Interactables; }
	bool SetCharacter() override;
};

