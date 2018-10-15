// Fill out your copyright notice in the Description page of Project Settings.

#include "Julia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/InteractInterface.h"

bool AJulia::TryUseElectronic(AActor * Target)
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
		}
		else if (GetController()->Role == ROLE_Authority)
		{
			AsInterface->I_Server_Use_Implementation(GetController()); // We're already server, no need for confirmation
		}
		AsInterface->I_Server_Use(GetController());
		return true;
	default: 
		return false;
	}
}

bool AJulia::GetLookedAtActor(TArray<AActor*>& OutActors)
{
	auto PC = Cast<ACustomPlayerController>(GetController());
	return PC->GetActorsInCenterOfScreen(OutActors); 
}
