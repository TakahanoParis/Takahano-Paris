// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Gameplay/CustomAIController.h"
#include "AICharacter.generated.h"

/**
 *	@class AAICharacter
 *	@brief Base class for AI character
 */
UCLASS()
class TAKAHANOPARIS_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	// Called at Spawn or level start
	void BeginPlay() override;

	/**
	 *	@property PatrolPath
	 *	@brief Path to follow
	 */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "IA")
	class ASplinePathActor * PatrolPath;

	/**
	 *	@fn	GetAIController()
	 *	@return The CustomAIController for that actor
	 */
	UFUNCTION()
		ACustomAIController * GetCustomAIController() const {return Cast<ACustomAIController>(GetController());}

	/**
	 *	@fn	GetAIController_BP()
	 *	@return The CustomAIController for that actor
	 *	@note For blueprints
	 */
	UFUNCTION(BlueprintPure, Category="AI", meta = (DisplayName = "Get Custom AI Controller"))
		ACustomAIController * GetCustomAIController_BP() const {return GetCustomAIController();}



	
};
