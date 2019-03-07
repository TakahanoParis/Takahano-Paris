// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractInterface.h"
#include "GameFramework/Controller.h"

EInteractableState IInteractInterface::I_GetInteractState() const
{
	return EInteractableState::ISE_Off;
}

bool IInteractInterface::I_Use(AController * User)
{
	I_Use_BP(User);
	return true;
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
