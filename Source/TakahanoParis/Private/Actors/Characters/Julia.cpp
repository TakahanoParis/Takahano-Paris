// Fill out your copyright notice in the Description page of Project Settings.

#include "Julia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/HackInterface.h"

AJulia::AJulia() : Super()
{
	HackDelegate.AddDynamic(this, &AJulia::Hack);
}




void AJulia::Tick(float DeltaSeconds)
{
	TArray<AActor*> ElectronicActors;
	GetLookedAtHackable(ElectronicActors);

}


bool AJulia::GetLookedAtHackable(TArray<AActor*>& OutActors) const
{

	auto PC = Cast<ACustomPlayerController>(GetController());
	const bool R = PC->GetActorsInCenterOfScreen<AActor>(OutActors);
	for (int32 id = OutActors.Num()-1; id >= 0; --id)
		if(!Cast<IHackInterface>(OutActors[id]))
			OutActors.RemoveAt(id,1,false);
	OutActors.Shrink();
	return R;
}

bool AJulia::TryHack(AActor* target) const 
{
	auto AsInterface = Cast<IHackInterface>(target);
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
			AsInterface->I_Server_Hack(GetController());
			return true;
		}
		if (GetController()->Role == ROLE_Authority)
		{
			AsInterface->I_Server_Hack_Implementation(GetController()); // We're already server, no need for confirmation
			return true;
		}
	default: 
		return false;
	}
	if(Cast<IHackInterface>(target))
	{
		HackDelegate.Broadcast(target);
		return true;
	}
	return false;

}

void AJulia::Hack(AActor * target)
{
	const auto AsInterface = Cast<IHackInterface>(target);
	AsInterface->I_Server_Hack(GetController());
}
