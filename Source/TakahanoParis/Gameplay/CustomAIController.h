// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"


class ASplinePathActor;

/**
 *	@class ACustomAIController
 *	@brief Base class for AI Controller. Adds a patrol action
 */
UCLASS()
class TAKAHANOPARIS_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

public:

	// Default Constructor
	ACustomAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/**
	 *	@fn GetPathActor()
	 *	@return the path used for this controller
	 */
	UFUNCTION()
	FORCEINLINE ASplinePathActor * GetPathActor() const {return PatrolPath;}

	/**
	 *	@fn SetPathActor()
	 *	@brief SetThePath to Use for this controller
	 */
	UFUNCTION()
	void SetPathActor(ASplinePathActor * NewPath) {PatrolPath = NewPath;}

	/**
	 *	@fn StartPatrol()
	 *	@brief Start the Patrol timer to launch Patrol
	 */
	UFUNCTION()
	virtual void StartPatrol();

	/**
	 *	@fn PausePatrol()
	 *	@brief pause the Patrol timer to launch Patrol
	 */
	UFUNCTION()
	void PausePatrol();

	/**
	 *	@fn PausePatrol()
	 *	@brief pause the Patrol timer to launch Patrol
	 */
	UFUNCTION()
	void EndPatrol();

protected:
	/**
	 *	@fn Patrol()
	 *	@brief Order the controlled pawn that moves
	 */
	UFUNCTION()
		void Patrol();

	
private:
	
	/**
	 *	@property PatrolPath
	 *	@brief The Path that the Controller will enforce on the Character
	 */
	UPROPERTY(Replicated, EditAnywhere, Category ="Path")
		ASplinePathActor * PatrolPath;

	/**
	 *	@property PathDistanceDelta
	 *	@brief the delta on the path it is supposed to follow
	 */
	UPROPERTY(Replicated)
		float PathDistanceDelta = 0.5;

	/**
	 *	@property PathAcceptanceRadius
	 *	@brief The radius of near target we're considering
	 */
	UPROPERTY(Replicated)
		float PathAcceptanceRadius = 0.5;


	/**
	 *	@property PatrolTimerHandle
	 *	@brief timer handle for the patrol 
	 */
	UPROPERTY()
	FTimerHandle PatrolTimerHandle;


	/**
	 *	@property PatrolTimerHandle
	 *	@brief timer handle for the patrol 
	 */
	UPROPERTY(Replicated, EditAnywhere)
	float TimerDelay;
	
	
	
};
