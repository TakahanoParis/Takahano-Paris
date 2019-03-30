// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScrollerJulia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "UnrealNetwork.h"
#include "TakahanoParis.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "Actors/Interfaces/HackInterface.h"
#include "TakahanoParisStatics.h"

ASideScrollerJulia::ASideScrollerJulia() : Super()
{
	//HackDelegate.AddDynamic(this, &ASideScrollerJulia::Hack);
	PrimaryActorTick.bCanEverTick = true;
	bIsUsingObject = false;
}




void ASideScrollerJulia::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetVisibleHackables();
}

void ASideScrollerJulia::MoveForward(float Value)
{
	if(bIsUsingObject)
	{
		if (UsedActor)
			return;
	}
	Super::MoveForward(Value);
}

void ASideScrollerJulia::MoveRight(float Value)
{
	if (bIsUsingObject)
	{
		if(UsedActor)	
			return;
	}
	Super::MoveRight(Value);
}

void ASideScrollerJulia::BeginPlay()
{
	Super::BeginPlay();
	Hackables.Empty();
	UGameplayStatics::GetAllActorsWithInterface(this, UHackInterface::StaticClass(), Hackables);
}


bool ASideScrollerJulia::SetVisibleHackables()
{
	const auto aPC = Cast<APlayerController>(GetController());
	if(aPC)
	{
		VisibleHackables = Hackables;
		ACustomPlayerController::GetVisibleActorsInArray(VisibleHackables, aPC);
	}
	return VisibleHackables.Num() > 0;
}



bool ASideScrollerJulia::TryHack(AActor* target)  
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

bool ASideScrollerJulia::Server_ReturnToCharacter_Validate()
{
	return true;
}

void ASideScrollerJulia::Server_ReturnToCharacter_Implementation()
{
	const auto aPC = Cast<APlayerController>(GetController());
	aPC->SetViewTargetWithBlend(this, 0.2);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
	bIsUsingObject = false;
	UsedActor = nullptr;
}

bool ASideScrollerJulia::Server_Hack_Validate(AActor * target)
{
	return true;
}


void ASideScrollerJulia::Server_Hack_Implementation (AActor * target)
{
	if (UTakahanoParisStatics::CallHackInterfaceOnActor(target, GetController()))
	{
		bIsUsingObject = true;
		UsedActor = target;
	}
}

bool ASideScrollerJulia::SetCharacter()
{
	bool IsValid = false;
	const auto julia = UTakahanoParisStatics::GetTakahanoParisJulia(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(julia, GetMesh());
		return true;
	}
	return false;
}



void ASideScrollerJulia::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASideScrollerJulia, bIsUsingObject);
	DOREPLIFETIME(ASideScrollerJulia, UsedActor);
	DOREPLIFETIME(ASideScrollerJulia, Hackables);
}
