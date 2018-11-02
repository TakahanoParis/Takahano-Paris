// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Gameplay/CustomAIController.h"
#include "AICharacter.generated.h"

class UAIPerceptionComponent;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIDetectionDelegate, AActor*, Actor, FAIStimulus, Stimulus);

/**
 *	@class AAICharacter
 *	@brief Base class for AI character
 *	@todo Use IGenericTeamAgentInterface or replace it all together by a Team Interface of my own
 */
UCLASS()
class TAKAHANOPARIS_API AAICharacter : public ABaseCharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	 /** Default UObject constructor. */
	AAICharacter(const FObjectInitializer& ObjectInitializer);

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
	 *	@return The CustomAIController for this actor
	 */
	UFUNCTION()
		ACustomAIController * GetCustomAIController() const {return Cast<ACustomAIController>(GetController());}

	/**
	 *	@fn	GetAIController_BP()
	 *	@return The CustomAIController for this actor
	 *	@note For blueprints
	 */
	UFUNCTION(BlueprintPure, Category="AI", meta = (DisplayName = "Get Custom AI Controller"))
		ACustomAIController * GetCustomAIController_BP() const {return GetCustomAIController();}

};
