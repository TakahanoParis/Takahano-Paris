// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"


ETeamAttitudeEnum FTeam::DefaultTeamAttitudeSolver(FTeam A, FTeam B)
{
		return A != B ? ETeamAttitudeEnum::TAE_Hostile : ETeamAttitudeEnum::TAE_Friendly;
}

const FTeam FTeam::NoTeam(FTeam::NoTeamId);

FTeam FTeam::GetTeamIdentifier(const AActor* TeamMember)
{
	const ITeamInterface * TeamAgent = Cast<const ITeamInterface>(TeamMember);
	if (TeamAgent)
	{
		return TeamAgent->I_GetTeam();
	}

	return FTeam::NoTeam;
}

ETeamAttitudeEnum FTeam::GetAttitude(const AActor* A, const AActor* B)
{
	const auto ATeam = Cast<ITeamInterface>(A);
	const auto BTeam = Cast<ITeamInterface>(B);
	if (!A || !B)
		return ETeamAttitudeEnum::TAE_Neutral;
	//return GetAttitude(ATeam->I_GetTeam(), BTeam->I_GetTeam());
	return ETeamAttitudeEnum::TAE_Neutral;
}


FTeam::FTeamAttitudeSolverFunction* FTeam::AttitudeSolverImpl = &DefaultTeamAttitudeSolver;

