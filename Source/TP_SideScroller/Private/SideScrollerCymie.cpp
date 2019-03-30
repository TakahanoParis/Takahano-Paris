// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScrollerCymie.h"
#include "Gameplay/CustomPlayerState.h"


bool ASideScrollerCymie::CanRun()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return false;
	if(aPC->GetStamina() >0)
		return true;
	return false;
}

void ASideScrollerCymie::Run()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return;
	if (aPC->UseStamina(RunDrain) > 0)
		const auto aPC2= Cast<ACustomPlayerState>(PlayerState);
		//Super::Run();
}

bool ASideScrollerCymie::SetCharacter()
{
	bool IsValid = false;
	const auto cymie = UTakahanoParisStatics::GetTakahanoParisCymie(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(cymie, GetMesh());
		return true;
	}
	return false;
}