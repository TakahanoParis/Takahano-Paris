// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeInterface.h"
#include "GameFramework/GameMode.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IGameModeInterface functions that are not pure virtual.
AController *  IGameModeInterface::AddSplitScreenPlayer_Implementation()
{
	
	const auto AsGM = Cast<AGameMode>(this);
	if (!AsGM)
		return nullptr;


	const int nPlayers = AsGM->GetNumPlayers();
	if (nPlayers <= 1)
	{
		return UGameplayStatics::CreatePlayer(AsGM, -1, true);
	}
	return UGameplayStatics::GetPlayerController(AsGM, 1);
}

ACharacter* IGameModeInterface::ReplaceCharacter_Implementation(AController* InController, TSubclassOf<ACharacter> NewPawnClass)
{
	const auto AsGM = Cast<AGameMode>(this);
	if (!AsGM)
		return nullptr;

	UWorld * World = InController->GetWorld();
	if (!World)
		return InController->GetCharacter();

	ACharacter * Pawn = InController->GetCharacter();
	if (Pawn)
		Pawn->Destroy();

	const auto start = AsGM->FindPlayerStart(InController);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const FTransform SpawnTransform = start->GetActorTransform();
	const auto PlayerNewPawn = Cast<ACharacter>(World->SpawnActor(NewPawnClass, &SpawnTransform, SpawnParams));
	InController->Possess(PlayerNewPawn);
	return PlayerNewPawn;
}

uint8 IGameModeInterface::GetDefaultPlayerTeamID_Implementation() const
{
	return 255;
}

uint8 IGameModeInterface::GetDefaultAITeamID_Implementation() const
{
	return 1;
}

