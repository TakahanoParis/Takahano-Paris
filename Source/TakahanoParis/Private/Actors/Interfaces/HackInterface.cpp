// Fill out your copyright notice in the Description page of Project Settings.

#include "HackInterface.h"

bool IHackInterface::I_Server_Hack_Validate(AController* User)
{
	return true;
}

void IHackInterface::I_Server_Hack_Implementation(AController* User)
{
	I_Hack(User);
}

void IHackInterface::I_TryHack_Implementation(AController* User)
{
	I_Server_Hack(User);
}
