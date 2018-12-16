// Fill out your copyright notice in the Description page of Project Settings.

#include "Cymie.h"
#include "Gameplay/CustomPlayerState.h"


bool ACymie::CanRun()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return false;
	if(aPC->GetStamina() >0)
		return true;
	return false;
}

void ACymie::Run()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return;
	if(aPC->UseStamina(RunDrain) > 0)
		Super::Run();
}
