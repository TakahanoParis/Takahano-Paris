// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"
#include "Actors/CCTVActor.h"
#include "Actors/Characters/AICharacter.h"
#include "UnrealNetwork.h"
#include "NavigationSystem.h"
#include "Gameplay/SplinePathActor.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CustomPlayerState.h"
#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Actors/Characters/AICharacter.h"


ACustomAIController::ACustomAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Basic setup :
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
	bWantsPlayerState = true;


	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACustomAIController::OnPerceptionReceived);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);


}

void ACustomAIController::BeginPlay()
{
	// call super begin play to have this function to work properly
	Super::BeginPlay();

	// Set up team ID
	const auto aGM = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(ACustomAIController::GetWorld()));
	if (aGM)
		ACustomAIController::Execute_I_Server_SetTeam(this, FTeam(aGM->GetDefaultAITeamID()));



	// Setup blackboard
	const bool IsBlackboardValid =  UseBlackboard(AIBlackboard, Blackboard);
	InitializeBlackboardValues(); // Anything could happen here, as blueprint can override this function

	// run the behavior tree
	if (BehaviourTreeAsset)
		RunBehaviorTree(BehaviourTreeAsset);


}

void ACustomAIController::OnPossess_Implementation(APawn* PossessedPawn)
{
	const bool IsBlackboardValid =  UseBlackboard(AIBlackboard, Blackboard);
	InitializeBlackboardValues(); // Anything could happen here, as blueprint can override this function

}

void ACustomAIController::StartPatrol()
{
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
	GetWorldTimerManager().SetTimer(PatrolTimerHandle, this, &ACustomAIController::Patrol, TimerDelay, true, 2.0f);

}

void ACustomAIController::PausePatrol()
{
	GetWorldTimerManager().PauseTimer(PatrolTimerHandle);
}

void ACustomAIController::EndPatrol()
{
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
}


FVector ACustomAIController::GetNextPointOnSpline(const float radius) 
{
	if (!PatrolPath)
		return FVector();
	FVector GoalLocation = PatrolPath->GetWorldLocationAlongSpline(PathDistanceDelta);
	
	if (GetPawn()->GetActorLocation().Equals(GoalLocation, radius))
	{
		// we need to move further up the spline
		PathDistanceDelta += PatrolPath->GetSpline()->GetSplineLength() / PatrolPath->GetPathPoints();
	}
	if (FMath::IsNearlyEqual(PatrolPath->GetSpline()->GetSplineLength(), PathDistanceDelta, radius))
	{
		// we need to reset the path
		PathDistanceDelta = 0;
	}
	return GoalLocation;
}

void ACustomAIController::Patrol()
{
	if(!GetPawn())
		return;

	// Check were are we compared to target
	const FVector GoalLocation = PatrolPath->GetWorldLocationAlongSpline(PathDistanceDelta);
	
	if(GetPawn()->GetActorLocation().Equals(GoalLocation,PathAcceptanceRadius))
	{
		// we need to move further up the spline
		PathDistanceDelta += PatrolPath->GetSpline()->GetSplineLength()/PatrolPath->GetPathPoints();
	
	}
	if(FMath::IsNearlyEqual( PatrolPath->GetSpline()->GetSplineLength(), PathDistanceDelta, PathAcceptanceRadius))
	{
		// we need to move further up the spline
		PathDistanceDelta = 0;
	}
	const auto result  = MoveToLocation(GoalLocation, PathAcceptanceRadius/2); // "/2" is a Quick and dirty fix to have a value smaller than our confirmation value
	const FString resultstring = (result != EPathFollowingRequestResult::Type::Failed)?TEXT("Success"):TEXT("Failed");


}

bool ACustomAIController::AttackActor(AActor* Target)
{
	if(!Target)
		return false;
	const auto Character = Cast<AAICharacter>(GetCharacter());
	if (!Character)
		return false;
	Character->Attack(Target);
	return true;
}

void ACustomAIController::InitializeBlackboardValues_Implementation()
{
	if (!GetBlackboardComponent())
		return;
	GetBlackboardComponent()->SetValueAsBool(TEXT("HasSpottedHostile"), false);
}

void ACustomAIController::I_SetTeam(FTeam NewTeam)
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (aPS)
		aPS->I_SetTeam(NewTeam);
}

FTeam ACustomAIController::I_GetTeam() const
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (aPS)
		return aPS->I_GetTeam();
	return 0;
}

void ACustomAIController::OnPerceptionReceived_Implementation(AActor* Actor, FAIStimulus Stimulus)
{
	if (!GetBlackboardComponent())
		return;
	const ETeamAttitudeEnum T = I_GetTeam().GetAttitude(Actor, this);
	switch (T)
	{
	case ETeamAttitudeEnum::TAE_Hostile:
		if (Stimulus.SensingSucceeded)
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("ActorPerceived"), Actor);
			OnHostileSpotted(Actor);
			break;
		}
		OnHostileSightLost(this, Stimulus.StimulusLocation);

		break;
	case ETeamAttitudeEnum::TAE_Friendly:
		// Implements friendly behaviour here
		break;
	case ETeamAttitudeEnum::TAE_Neutral:
		// Implements friendly behaviour here
		break;
	}
}

void ACustomAIController::OnHostileSpotted_Implementation(const AActor * Actor)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("HasSpottedHostile"), true);
	UE_LOG(LogTemp, Warning, TEXT("%s sees %s as hostile"), *this->GetName(), *Actor->GetName());
}

void ACustomAIController::OnHostileSightLost_Implementation(const AActor * Actor, const FVector &LastSeenPosition)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("HasSpottedHostile"), false);
	UE_LOG(LogTemp, Warning, TEXT("%s lost sight of %s as hostile"), *this->GetName(), *Actor->GetName());
}


void ACustomAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomAIController, PatrolPath);
	DOREPLIFETIME(ACustomAIController, TimerDelay);
	DOREPLIFETIME(ACustomAIController, PathDistanceDelta);
	DOREPLIFETIME(ACustomAIController, PathAcceptanceRadius);
}
