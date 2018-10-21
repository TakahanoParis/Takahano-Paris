// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"
#include "Actors/CCTVActor.h"
#include "UnrealNetwork.h"
#include "NavigationSystem.h"
#include "SplinePathActor.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "TimerManager.h"


ACustomAIController::ACustomAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	UE_LOG(LogTemp, Error, TEXT("Pawn : %s"),  *GetPawn()->GetActorLocation().ToString())
	// Check were are we compared to target
	const FVector GoalLocation = PatrolPath->GetWorldLocationAlongSpline(PathDistanceDelta);
	if(GetPawn()->GetActorLocation().Equals(GoalLocation,PathAcceptanceRadius))
	{
		// we need to move further up the spline
		PathDistanceDelta += PatrolPath->GetSpline()->GetSplineLength()/PatrolPath->GetPathPoints();
		UE_LOG(LogTemp, Warning, TEXT("PathDistanceDelta : %f"), PathDistanceDelta);
	
	}
	if(FMath::IsNearlyEqual( PatrolPath->GetSpline()->GetSplineLength(), PathDistanceDelta, PathAcceptanceRadius))
	{
		// we need to move further up the spline
		PathDistanceDelta = 0;
		UE_LOG(LogTemp, Warning, TEXT("reset spline progress"));
	}
	const auto result  = MoveToLocation(GoalLocation, PathAcceptanceRadius/2); // "/2" is a Quick and dirty fix to have a value smaller than our confirmation value
	const FString resultstring = (result != EPathFollowingRequestResult::Type::Failed)?TEXT("Success"):TEXT("Failed");
	UE_LOG(LogTemp, Warning, TEXT("Patrol : %s, spline distance target :%f"),*resultstring, FVector::Distance(PatrolPath->GetWorldLocationAlongSpline(PathDistanceDelta), GetPawn()->GetActorLocation()));

}


void ACustomAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomAIController, PatrolPath);
	DOREPLIFETIME(ACustomAIController, TimerDelay);
	DOREPLIFETIME(ACustomAIController, PathDistanceDelta);
	DOREPLIFETIME(ACustomAIController, PathAcceptanceRadius);
}