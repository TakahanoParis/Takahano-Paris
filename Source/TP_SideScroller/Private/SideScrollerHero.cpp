// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScrollerHero.h"
#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/CustomPlayerState.h"
#include "TP_SideScroller.h"


ASideScrollerHero::ASideScrollerHero() : Super()
{
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// Move freely
	bUseControllerRotationYaw = false;
}


void ASideScrollerHero::BeginPlay()
{
	Super::BeginPlay();
	SetInteractableActors();
}

void ASideScrollerHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//SetVisibleInteractableActors();
}

void ASideScrollerHero::Use()
{
	SetVisibleInteractableActors();
	UE_LOG(LogTP_SideScroller, Display, TEXT("%s Use"), *GetName());
	/** Interact with climbable */
	if (GetCanClimb() && ClimbableActor)
		Climb(ClimbableActor);
}

void ASideScrollerHero::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASideScrollerHero::Use);
}

bool ASideScrollerHero::TryUse(AActor * Target)
{
	auto AsInterface = Cast<IInteractInterface>(Target);
	if(!AsInterface)
		return false;
	switch(AsInterface->I_GetInteractState()) 
	{ 
	case EInteractableState::ISE_Off: 
		return false;
	case EInteractableState::ISE_Locked: 
		return false;
	case EInteractableState::ISE_Used: 	
		return false;
	case EInteractableState::ISE_Useable:
		if (GetController()->Role == ROLE_AutonomousProxy)
		{
			AsInterface->I_Server_Use(GetController());
			return true;
		}
		if (GetController()->Role == ROLE_Authority)
		{
			AsInterface->I_Server_Use_Implementation(GetController()); // We're already server, no need for confirmation
			return true;
		}
	default: 
		return false;
	}
}

bool ASideScrollerHero::I_TakeDamage(const float& DamageAmount, AActor* Instigator)
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	aPS->I_TakeDamage(DamageAmount, Instigator);
	return true;
}

AActor * ASideScrollerHero::GetClosestInteractableActor(float &Distance) const
{
	const auto Actors = GetAllInteractableActors();
	Distance = FVector::Distance(GetActorLocation(), Actors[0]->GetActorLocation());
	auto OutActor = Actors[0];
	for (auto it : Actors)
	{
		const auto T = FVector::Distance(GetActorLocation(), it->GetActorLocation());
		if (T < Distance)
		{
			Distance = T;
			OutActor = it;
		}
		
	}
	return OutActor;
}

void ASideScrollerHero::SetInteractableActors()
{
	UGameplayStatics::GetAllActorsWithInterface(this, UInteractInterface::StaticClass(), InteractableActors);
}

void ASideScrollerHero::SetVisibleInteractableActors()
{
	const auto aPC = Cast<ACustomPlayerController>(GetController());
	if(aPC)
	{
		VisibleInteractableActors.Empty();
		VisibleInteractableActors = InteractableActors;
		ACustomPlayerController::GetVisibleActorsInArray(VisibleInteractableActors, aPC);
	}

}



