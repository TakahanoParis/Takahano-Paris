// Fill out your copyright notice in the Description page of Project Settings.

#include "Julia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/HackInterface.h"
#include "UnrealNetwork.h"
#include "TakahanoParis.h"
#include "Kismet/GameplayStatics.h"

AJulia::AJulia() : Super()
{
	//HackDelegate.AddDynamic(this, &AJulia::Hack);
	PrimaryActorTick.bCanEverTick = true;
	bIsUsingObject = false;
}




void AJulia::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetVisibleHackables();
}

void AJulia::MoveForward(float Value)
{
	if(bIsUsingObject)
	{
		if (UsedActor)
			return;
	}
	Super::MoveForward(Value);
}

void AJulia::MoveRight(float Value)
{
	if (bIsUsingObject)
	{
		if(UsedActor)	
			return;
	}
	Super::MoveRight(Value);
}

void AJulia::BeginPlay()
{
	Super::BeginPlay();
	Hackables.Empty();
	UGameplayStatics::GetAllActorsWithInterface(this, UHackInterface::StaticClass(), Hackables);
}


bool AJulia::SetVisibleHackables()
{
	const auto aPC = Cast<APlayerController>(GetController());
	if(aPC)
	{
		VisibleHackables = Hackables;
		ACustomPlayerController::GetVisibleActorsInArray(VisibleHackables, aPC);
	}
	return VisibleHackables.Num() > 0;
}



bool AJulia::TryHack(AActor* target)  
{
	auto AsInterface = Cast<IHackInterface>(target);
	HackDelegate.Broadcast(target);
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
			Server_Hack(target);
			return true;
		}
		if (GetController()->Role == ROLE_Authority)
		{
			Server_Hack_Implementation(target);
			return true;
		}
	default: 
		return false;
	}
}

bool AJulia::Server_ReturnToCharacter_Validate()
{
	return true;
}

void AJulia::Server_ReturnToCharacter_Implementation()
{
	const auto aPC = Cast<APlayerController>(GetController());
	aPC->SetViewTargetWithBlend(this, 0.2);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
	bIsUsingObject = false;
	UsedActor = nullptr;
}

bool AJulia::Server_Hack_Validate(AActor * target)
{
	return true;
}


void AJulia::Server_Hack_Implementation (AActor * target)
{
	const auto AsInterface = Cast<IHackInterface>(target);
	if (!AsInterface)
		return;
	bIsUsingObject = true;
	UsedActor = target;
	if (GetController())
		AsInterface->Execute_I_Server_Hack(target, GetController());
}

void AJulia::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJulia, bIsUsingObject);
	DOREPLIFETIME(AJulia, UsedActor);
	DOREPLIFETIME(AJulia, Hackables);
}
