// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterTeamInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"

FTeam ICharacterTeamInterface::I_GetTeam() const
{
	const auto AsCharacter = Cast<ACharacter>(this);
	if (AsCharacter->GetController())
		if (AsCharacter->GetController()->PlayerState)
		{
			const auto aPS = Cast<ACustomPlayerState>(AsCharacter->GetController()->PlayerState);
			if (aPS)
				return aPS->I_GetTeam();
		}
	return 0;
}


void ICharacterTeamInterface::I_SetTeam(FTeam NewTeam)
{
	const auto AsCharacter = Cast<ACharacter>(this);
	const auto aPS = Cast<ACustomPlayerState>(AsCharacter->GetController()->PlayerState);
	if (aPS)
		return aPS->I_SetTeam(NewTeam);
}
