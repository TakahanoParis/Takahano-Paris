// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameMode.h"



ETeamAttitudeEnum DefaultTeamAttitudeSolver(FTeam A, FTeam B)
{
		return A != B ? ETeamAttitudeEnum::TAE_Hostile : ETeamAttitudeEnum::TAE_Friendly;
}

FTeam::FTeamAttitudeSolverFunction* FTeam::AttitudeSolverImpl = &DefaultTeamAttitudeSolver;
