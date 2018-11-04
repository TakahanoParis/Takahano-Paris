// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"
#include "Actors/CCTVActor.h"
#include "Actors/Characters/AICharacter.h"
#include "UnrealNetwork.h"
#include "NavigationSystem.h"
#include "SplinePathActor.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"


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
	
	// Setup blackboard
	const bool IsBlackboardValid =  UseBlackboard(AIBlackboard, Blackboard);
	InitializeBlackboardValues(); // Anything could happen here, as blueprint can override this function

	// run the behavior tree
	if (BehaviourTreeAsset)
		RunBehaviorTree(BehaviourTreeAsset);
}

void ACustomAIController::OnPossess_Implementation(APawn* PossessedPawn)
{

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



#if 0
	// setting static variables for patrol
	static float distance;
	const auto pawn= Cast<ACharacter>(GetPawn());
	if (!pawn)
		return;
	static const float patrolSpeed = pawn->GetCharacterMovement()->MaxWalkSpeed / TimerDelay;
	
	distance += patrolSpeed;
	if (distance > PatrolPath->GetLength())
		distance -= PatrolPath->GetLength();
	// do the patrolling 
	const auto result = MoveToLocation(PatrolPath->GetWorldLocationAlongSpline(distance), PathAcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("distance : %f"), distance);

#endif // 0
	
	
}

void ACustomAIController::InitializeBlackboardValues_Implementation()
{

}

void ACustomAIController::I_SetTeam(FTeam NewTeam)
{
}

FTeam ACustomAIController::I_GetTeam() const
{
	return FTeam();
}

void ACustomAIController::OnPerceptionReceived_Implementation(AActor* Actor, FAIStimulus Stimulus)
{
	const ETeamAttitudeEnum T = I_GetTeam().GetAttitude(Actor, this);

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeamAttitudeEnum"), true);
	if (EnumPtr) 
		UE_LOG(LogTemp, Warning, TEXT("%s sees %s as %d"), *this->GetName(), *Actor->GetName(), *EnumPtr->GetNameByValue((int8)T).ToString());
}


void ACustomAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomAIController, PatrolPath);
	DOREPLIFETIME(ACustomAIController, TimerDelay);
	DOREPLIFETIME(ACustomAIController, PathDistanceDelta);
	DOREPLIFETIME(ACustomAIController, PathAcceptanceRadius);
}
