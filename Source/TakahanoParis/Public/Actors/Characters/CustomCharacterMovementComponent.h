// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 *	@class UCustomCharacterMovementComponent
 *	@brief Improve the UCharacterMovementComponent by adding a curve for walk speed 
 */
UCLASS()
class TAKAHANOPARIS_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 *	@property WalkSpeedCurve
	 *	@brief curve that define Walk/run speed based on a delta 
	 */
	UPROPERTY(Category = "Character Movement: Walking|curve", EditAnywhere, BlueprintReadWrite)
		UCurveFloat* WalkSpeedCurve;

	/**
	 *	@property bDeltaCalculatedInBlueprint
	 *	@brief boolean flag that show that SpeedDelta is calculated through the BP implementable event
	 */
	UPROPERTY(Category = "Character Movement: Walking|curve", EditAnywhere, BlueprintReadWrite)
		bool bDeltaCalculatedInBlueprint = false;

	/**
	 *	@property SpeedDelta
	 *	@brief The Speed Delta calculated by @see GetSpeedDelta() function
	 */
	UPROPERTY(Replicated, Category = "Character Movement: Walking|curve", VisibleAnywhere, BlueprintReadOnly)
		float SpeedDelta;

	/**
	 *	@property NormalSpeedDelta
	 *	@brief The Speed Delta to use when moving at normal speed
	 */
	UPROPERTY(Replicated, Category = "Character Movement: Walking|curve", EditAnywhere, BlueprintReadOnly)
		float NormalSpeedDelta = 0.5;

	/**
	 *	@property bIsRunning
	 *	@brief Boolean flag telling to use max speed (ie. 1.0 Delta)
	 */
	UPROPERTY(Replicated, Category = "Character Movement: Walking|curve", EditAnywhere, BlueprintReadWrite)
		bool bIsRunning = false;

	/**
	 *	@property bIsRunning
	 *	@brief how fast we change from one value to the next.
	 */
	UPROPERTY(Replicated, Category = "Character Movement: Walking|curve", EditAnywhere, BlueprintReadOnly)
		float RunInterpolationSpeed = 1.0;

	/**
	 *	@fn GetSpeedDelta()
	 *	@brief implements the calculus behind the SpeedDelta
	 */
	UFUNCTION(Category = "Character Movement: Walking", BlueprintNativeEvent, meta = (DisplayName = "GetSpeedDelta"))
		float GetSpeedDelta() const;

	//overriden from CharacterMovementComponent
	virtual float GetMaxSpeed() const override;
	void PerformMovement(float DeltaTime) override;

};
