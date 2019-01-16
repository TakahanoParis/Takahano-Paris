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
#include "BrainComponent.h"
#include "Actors/Characters/Hero.h"
#include "Kismet/KismetMathLibrary.h"


ACustomAIController::ACustomAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Basic setup :
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
	bWantsPlayerState = true;

#if 0
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	SetPerceptionComponent(*AIPerception);

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACustomAIController::OnPerceptionReceived);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACustomAIController::OnPerceptionUpdated);
//	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACustomAIController::OnPerceptionReceived);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

#endif //0
}

void ACustomAIController::BeginPlay()
{
	// call super begin play to have this function to work properly
	Super::BeginPlay();

	// Set up team ID
	if (!GetWorld())
		return;
	const auto aGM = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (aGM)
		ACustomAIController::Execute_I_Server_SetTeam(this, FTeam(aGM->GetDefaultAITeamID()));

	// Setup blackboard
	const bool IsBlackboardValid =  UseBlackboard(AIBlackboard, Blackboard);
	InitializeBlackboardValues(); // Anything could happen here, as blueprint can override this function

	// run the behavior tree
	if (BehaviourTreeAsset)
		RunBehaviorTree(BehaviourTreeAsset);

	if(IsBlackboardValid)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this);
	}

}

void ACustomAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//CheckLastPerception();
}

void ACustomAIController::OnPossess_Implementation(APawn* PossessedPawn)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("Enable"), true);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRate"), 1.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 10.f);
	const bool IsBlackboardValid =  UseBlackboard(AIBlackboard, Blackboard);
	InitializeBlackboardValues(); // Anything could happen here, as blueprint can override this function
	if (IsBlackboardValid)
	{
		const auto Pawn = Cast<AAICharacter>(PossessedPawn);
		if(Pawn)
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRate"), Pawn->GetRate());
			GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), Pawn->GetRange());
		}
	}
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

void ACustomAIController::GetHostilesInMap(TArray<AActor*> &Out, const AActor* WorldContext, const FTeam &FriendlyTeam)
{
	Out.Empty();
	if (!WorldContext)
		return;
	TArray<AActor*> TeamActors;
	UGameplayStatics::GetAllActorsWithInterface(WorldContext, UTeamInterface::StaticClass(), TeamActors);
	for(auto it : TeamActors)
	{
		
		if (FTeam::GetAttitude(it, FriendlyTeam) == ETeamAttitudeEnum::TAE_Hostile)
			Out.Add(it);
	}
}

bool ACustomAIController::ActorIsFullyVisible(const AActor * In)
{
	APawn * Pawn = GetPawn();

	// Trace parameters
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	const FName TraceTag("VisibilityTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	Params.TraceTag = TraceTag;
	Params.AddIgnoredActor(Pawn);

	FVector ViewLocation = Pawn->GetActorLocation();
	FRotator ViewRotator = Pawn->GetActorRotation();

	const FVector ViewVector = UKismetMathLibrary::GetForwardVector(ViewRotator);
	FVector ToOtherActor = In->GetActorLocation() - ViewLocation;
	ToOtherActor.Normalize();

	// Forget about actors not in view cone :
	if (FVector::DotProduct(ToOtherActor, ViewVector) < FMath::Cos(FieldOfView)) // lets hope FMath is in degrees, not Radians 
	{
		return false;
	}


	// First trace
	struct FHitResult OutHit;
	Params.AddIgnoredActor(In);
	GetWorld()->LineTraceSingleByChannel(OutHit, ViewLocation, In->GetActorLocation(), ECollisionChannel::ECC_Visibility, Params);
	if (OutHit.IsValidBlockingHit())
		return false;

	// Second Trace :

	FVector Origin, Extend;
	In->GetActorBounds(false, Origin, Extend);
	const FCollisionShape BoundingShape = FCollisionShape::MakeBox(Extend);
	GetWorld()->SweepSingleByChannel(OutHit, ViewLocation, In->GetActorLocation(), In->GetActorQuat(), ECollisionChannel::ECC_Visibility, BoundingShape, Params);
	if (OutHit.IsValidBlockingHit())
		return false;

	return true;
	
}

TArray<AActor*> ACustomAIController::GetFullyVisibleActorsInArray(const TArray<AActor*>& In)
{
	TArray<AActor*> Out;
	APawn * Pawn = GetPawn();

	// Trace parameters
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	const FName TraceTag("VisibilityTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	Params.TraceTag = TraceTag;
	Params.AddIgnoredActor(Pawn);

	for (auto it : In)
	{
		FVector ViewLocation = Pawn->GetActorLocation();
		FRotator ViewRotator = Pawn->GetActorRotation();
		
		const FVector ViewVector = UKismetMathLibrary::GetForwardVector(ViewRotator);
		FVector ToOtherActor = it->GetActorLocation() - ViewLocation;
		ToOtherActor.Normalize();

		// Forget about actors not in view cone :
		if (FVector::DotProduct(ToOtherActor, ViewVector) < FMath::Cos(FieldOfView)) // lets hope FMath is in degrees, not Radians 
		{
			continue;
		}


		 // First trace
		struct FHitResult OutHit;
		Params.AddIgnoredActor(it);
		GetWorld()->LineTraceSingleByChannel(OutHit, ViewLocation,  it->GetActorLocation(), ECollisionChannel::ECC_Visibility, Params);
		if (OutHit.IsValidBlockingHit())
			continue;

		// Second Trace :

		FVector Origin, Extend;
		it->GetActorBounds(false, Origin, Extend);
		const FCollisionShape BoundingShape = FCollisionShape::MakeBox(Extend);
		GetWorld()->SweepSingleByChannel(OutHit, ViewLocation, it->GetActorLocation(), it->GetActorQuat(), ECollisionChannel::ECC_Visibility,  BoundingShape, Params);
		if (OutHit.IsValidBlockingHit())
			continue;

		Out.Add(it);

	}
	return Out;
}

bool ACustomAIController::Server_StopAILogic_Validate(bool bStop)
{
	return true;
}

void ACustomAIController::Server_StopAILogic_Implementation(bool bStop)
{
	bLogicIsDisabled = bStop;
	SwitchToDisabledState(bLogicIsDisabled);
}

void ACustomAIController::OnRep_DisableLogic()
{

	if (bLogicIsDisabled)
	{
		BrainComponent->PauseLogic(FString("Disabled"));
		return;
	}
	BrainComponent->ResumeLogic(FString("Enabled"));
}

void ACustomAIController::SwitchToDisabledState(const bool &bNewState)
{
	if (!GetBlackboardComponent())
		return;
	GetBlackboardComponent()->SetValueAsBool(TEXT("Enable"), bNewState);
}

#if 0

void ACustomAIController::CheckLastPerception()
{

	if (!GetPerceptionComponent())
		return;

	if (PerceivedActors.Num() == 0)
		return;

	for (int id = PerceivedActors.Num() -1; id >= 0; --id)
	{
		auto it = PerceivedActors[id];
		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(it, Info);
		const auto Stimulus = Info.LastSensedStimuli[0];
		if (!Stimulus.IsExpired())
			continue;
		UE_LOG(LogTemp, Display, TEXT("stimulus died of age : %f "), Stimulus.GetAge());

		const ETeamAttitudeEnum T = I_GetTeam().GetAttitude(it, this);
		switch (T)
		{
		case ETeamAttitudeEnum::TAE_Hostile:
			OnHostileSightLost(it, Stimulus.StimulusLocation);
			break;

		case ETeamAttitudeEnum::TAE_Friendly:
			OnFriendlySightLost(it, Stimulus.StimulusLocation);
			break;

		case ETeamAttitudeEnum::TAE_Neutral:
			break;
		}


		PerceivedActors.RemoveAt(id);
	}
}

void ACustomAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

	

}


void ACustomAIController::OnPerceptionReceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (!GetBlackboardComponent())
		return;

	if (!Stimulus.WasSuccessfullySensed())
	{
		const ETeamAttitudeEnum T = I_GetTeam().GetAttitude(Actor, this);
		switch (T)
		{
		case ETeamAttitudeEnum::TAE_Hostile:
			OnHostileSightLost(Actor, Actor->GetActorLocation());
			break;

		case ETeamAttitudeEnum::TAE_Friendly:
			OnFriendlySightLost(Actor, Actor->GetActorLocation());
			break;

		case ETeamAttitudeEnum::TAE_Neutral:
			break;
		};
		return;
	}


	if (!PerceivedActors.Contains(Actor))
		PerceivedActors.Add(Actor);
	const ETeamAttitudeEnum T = I_GetTeam().GetAttitude(Actor, this);
	switch (T)
	{
	case ETeamAttitudeEnum::TAE_Hostile:
		OnHostileSpotted(Actor);
		break;
	case ETeamAttitudeEnum::TAE_Friendly:
		OnFriendlySpotted(this);
		break;
	case ETeamAttitudeEnum::TAE_Neutral:
		break;
	}
}

void ACustomAIController::OnHostileSpotted(const AActor * Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s sees %s as hostile"), *this->GetName(), *Actor->GetName());

	GetBlackboardComponent()->SetValueAsBool(TEXT("HasSpottedHostile"), true);
	const auto PlayerHero = Cast<AHero>(Actor);
	if (PlayerHero)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("ActorPerceived"), (UObject*)PlayerHero);
		GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyActorLocation"), PlayerHero->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), (UObject*)PlayerHero);
	}

	// call blueprint function
	OnHostileSpotted_BP(Actor);
}

void ACustomAIController::OnHostileSightLost(AActor*const Actor, const FVector& LastSeenPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("%s lost sight of %s as hostile"), *this->GetName(), *Actor->GetName());
	GetBlackboardComponent()->SetValueAsBool(TEXT("HasSpottedHostile"), false);

	GetBlackboardComponent()->SetValueAsObject(TEXT("ActorPerceived"), nullptr);
	GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyActorLocation"), LastSeenPosition);

	GetBlackboardComponent()->SetValueAsBool(TEXT("RecentlySighted"), true);
	// call blueprint function
	OnHostileSightLost_BP(Actor, LastSeenPosition);
}

void ACustomAIController::OnFriendlySpotted(const AActor * Actor)
{
	//UE_LOG(LogTemp, Display, TEXT("%s sees %s as friendly"), *this->GetName(), *Actor->GetName());
	OnFriendlySpotted_BP(Actor);
}

void ACustomAIController::OnFriendlySightLost(const AActor * Actor, const FVector &LastSeenPosition)
{
	//UE_LOG(LogTemp, Display, TEXT("%s lost sight of %s as friendly"), *this->GetName(), *Actor->GetName());
	OnFriendlySightLost_BP(Actor, LastSeenPosition);
}



void ACustomAIController::OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!GetBlackboardComponent())
		return;

		if(!Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		}
}
#endif //PERCEPTION_AI

void ACustomAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomAIController, PatrolPath);
	DOREPLIFETIME(ACustomAIController, TimerDelay);
	DOREPLIFETIME(ACustomAIController, PathDistanceDelta);
	DOREPLIFETIME(ACustomAIController, PathAcceptanceRadius);
	DOREPLIFETIME(ACustomAIController, bLogicIsDisabled);
}