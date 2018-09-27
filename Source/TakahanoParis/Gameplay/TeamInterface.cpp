// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameMode.h"

int ITeamInterface::I_GetTeamBP()
{
	return I_GetTeam();
}


void ITeamInterface::I_SetTeamBP(uint8 NewTeam)
{
	I_SetTeam(NewTeam);
}

bool ITeamInterface::I_ValidateTeam(FTeam Team, const UObject * WorldContextObject)
{

	return false;
	/*
	AGameModeBase * GM = UGameplayStatics::GetGameMode(WorldContextObject);
	if (!GM)
		return false;
	AAnimaGameMode * AnimaGM = Cast<AAnimaGameMode>(GM);
	if(!AnimaGM)
		return false;
	if (AnimaGM->TeamExists(Team.TeamNumber))
	{
		// Correct the Name and the Color from GameMode
		Team.TeamName = AnimaGM->GetTeam(Team.TeamNumber).TeamName;
		Team.TeamColor = AnimaGM->GetTeam(Team.TeamNumber).TeamColor;
		return true;
	}
	//Can implement default behaviour here.
	return false;
	*/
}
