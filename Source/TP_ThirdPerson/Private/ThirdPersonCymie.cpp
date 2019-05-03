// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonCymie.h"
#include "Gameplay/CustomPlayerState.h"


AThirdPersonCymie::AThirdPersonCymie() : Super()
{

}

bool AThirdPersonCymie::CanRun()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return false;
	if(aPC->GetStamina() >0)
		return true;
	return false;
}

void AThirdPersonCymie::Run()
{
	const auto aPC = Cast<ACustomPlayerState>(PlayerState);
	if (!aPC)
		return;
	if(aPC->UseStamina(RunDrain) > 0)
		Super::Run();
}

bool AThirdPersonCymie::SetCharacter()
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