// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractInterface.h"
#include "GameFramework/Controller.h"


bool IInteractInterface::I_Use_BP_Implementation(AController* User)
{
	return I_Use(User);
}

bool IInteractInterface::I_Server_Use_Validate(AController * User)
{
	//Just Check CDState
	return (I_GetInteractState() == EInteractableState::ISE_Useable && User);
}

void IInteractInterface::I_Server_Use_Implementation(AController * User)
{
	I_Use(User);
}
