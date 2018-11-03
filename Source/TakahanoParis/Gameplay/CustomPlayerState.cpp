// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerState.h"
#include "UnrealNetwork.h"


void ACustomPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(ACustomPlayerState, HeroTeam);
}