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

	
private:
	
	/**
	 *	@property PatrolPath
	 *	@brief The Path that the Controller will enforce on the Character
	 */
	UPROPERTY(Replicated, EditAnywhere, Category ="Path")
		ASplinePathActor * PatrolPath;

	
	
	
};
