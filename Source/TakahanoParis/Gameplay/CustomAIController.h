// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actors/Interfaces/TeamInterface.h"
#include "CustomAIController.generated.h"


class ASplinePathActor;

/**
 *	@class ACustomAIController
 *	@brief Base class for AI Controller. Adds a patrol action
 */
UCLASS()
class TAKAHANOPARIS_API ACustomAIController : public AAIController, public ITeamInterface
{
	GENERATED_BODY()

public:


	// Default Constructor
	ACustomAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// called at game start
	void BeginPlay() override;

	// Called when starting to take control of pawn
	virtual void OnPossess_Implementation(APawn* PossessedPawn);


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
	 *	@fn StartPatrol_BP()
	 *	@brief Start the Patrol timer to launch Patrol
	 *	@note for blueprints
	 */
	UFUNCTION(BlueprintCallable, Category="AI", meta = (DisplayName = "Start Patrol"))
		void StartPatrol_BP() {StartPatrol();}




	/**
	 *	@fn PausePatrol()
	 *	@brief pause the Patrol timer
	 */
	UFUNCTION()
	void PausePatrol();

	/**
	 *	@fn PausePatrol()
	 *	@brief pause the Patrol timer
	 */
	UFUNCTION(BlueprintCallable, Category = "AI", meta = (DisplayName = "Pause Patrol"))
		void PausePatrol_BP() { PausePatrol(); }

	/**
	 *	@fn EndPatrol()
	 *	@brief end the Patrol timer
	 */
	UFUNCTION()
	void EndPatrol();

	/**
 *	@fn EndPatrol_BP()
 *	@brief end the Patrol timer
 */
	UFUNCTION(BlueprintCallable, Category = "AI", meta = (DisplayName = "End Patrol"))
		void EndPatrol_BP() { EndPatrol(); }

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
		float PathDistanceDelta = 0;

	/**
	 *	@property PathAcceptanceRadius
	 *	@brief The radius of near target we're considering
	 */
	UPROPERTY(Replicated)
		float PathAcceptanceRadius = 200;


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
	float TimerDelay = 0.3f;


protected:
	/// blackboard
	/**
	 *	@property AIBlackboard
	 *	@brief Blackboard for the controller to use
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
	class UBlackboardData * AIBlackboard;


	/**
	 *	@property BehaviourTreeAsset
	 *	@brief the Behaviour tree that defines what the character/controller will do
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
		class UBehaviorTree  * BehaviourTreeAsset;

	/**
	 *	@fn InitializeBlackboardValues()
	 *	@brief Initialize the blackboard with important values
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI|Blackboard")
		void InitializeBlackboardValues();
	virtual void InitializeBlackboardValues_Implementation();

protected:

	/**
	 *	@property sightConfig
	 *	@brief the config for the view detection for the controller
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Perception", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight * SightConfig;





	/**
	 *	@fn OnPerceptionReceived()
	 *	@brief Function handles the arrival of a Stimuli on the perception component
	 *	@param Target  : the thing that was spotted
	 *	@param Stimulus : the structy containing details about that perception
	 */
	UFUNCTION(BlueprintNativeEvent)
		void OnPerceptionReceived(AActor * Actor, FAIStimulus Stimulus);


	/**
	 *	@property AISightRadius
	 *	@brief How far it can see you
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Sight Age"))
		float AISightRadius = 500.0f;

	/**
	 *	@property AISightAge
	 *	@brief How long he remember seeing you
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Sight Age"))
		float AISightAge = 5.0f;

	/**
	 *	@property AILoseSightRadius
	 *	@brief At which point you're not into view anymore 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Lose Sight Radius"))
		float AILoseSightRadius = AISightRadius + 50.0f;

	/**
	 *	@property AIFieldOfView
	 *	@brief How wide it sees
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Field Of View"))
		float AIFieldOfView = 90.0f;



	// inherited via @see ITeamInterface
	virtual void I_SetTeam(FTeam NewTeam) override;
	virtual FTeam I_GetTeam() const;

};
