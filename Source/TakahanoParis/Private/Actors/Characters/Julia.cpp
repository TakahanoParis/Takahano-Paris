// Fill out your copyright notice in the Description page of Project Settings.

#include "Julia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/HackInterface.h"
#include "UnrealNetwork.h"

AJulia::AJulia() : Super()
{
	//HackDelegate.AddDynamic(this, &AJulia::Hack);
	//bCanEverTick = true;
}




void AJulia::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TArray<AActor*> ElectronicActors;
	GetLookedAtHackable(ElectronicActors);

}

void AJulia::MoveForward(float Value)
{
	if(bIsUsingObject)
	{
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


bool AJulia::GetLookedAtHackable(TArray<AActor*>& OutActors) const
{

	auto PC = Cast<ACustomPlayerController>(GetController());
	if (!PC)
		return false;
	const bool R = PC->GetActorsInCenterOfScreen<AActor>(OutActors);
	for (int32 id = OutActors.Num()-1; id >= 0; --id)
		if(!Cast<IHackInterface>(OutActors[id]))
			OutActors.RemoveAt(id,1,false);
	OutActors.Shrink();
	return R;
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

void AJulia::ReturnToCharacter()
{
	const auto aPC = Cast<APlayerController>(GetController());
	aPC->SetViewTargetWithBlend(this, 0.2);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
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
}
