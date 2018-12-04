// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"


ETeamAttitudeEnum FTeam::DefaultTeamAttitudeSolver(FTeam A, FTeam B)
{
		return A.GetId() != B.GetId() ? ETeamAttitudeEnum::TAE_Hostile : ETeamAttitudeEnum::TAE_Friendly;
}


bool ITeamInterface::I_Server_SetTeam_Validate(FTeam NewTeam)
{
	return true;
}
void ITeamInterface::I_Server_SetTeam_Implementation(FTeam NewTeam)
{
	I_SetTeam(NewTeam);
}

ETeamAttitudeEnum ITeamInterface::I_GetAttitudeToward(AActor* OtherTeamActor) const
{
	const auto AsTeamInterface= Cast<ITeamInterface>(OtherTeamActor);
	if(AsTeamInterface)
		return FTeam::GetAttitude(this->I_GetTeam(), AsTeamInterface->I_GetTeam());
	return ETeamAttitudeEnum::TAE_Neutral;
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

const ETeamAttitudeEnum FTeam::GetAttitude(const AActor* A, const AActor* B)
{
	const auto ATeam = Cast<ITeamInterface>(A);
	const auto BTeam = Cast<ITeamInterface>(B);
	if (!ATeam || !BTeam)
		return ETeamAttitudeEnum::TAE_Neutral;
	return GetAttitude(ATeam->I_GetTeam(), BTeam->I_GetTeam());
}


FTeam::FTeamAttitudeSolverFunction* FTeam::AttitudeSolverImpl = &DefaultTeamAttitudeSolver;

