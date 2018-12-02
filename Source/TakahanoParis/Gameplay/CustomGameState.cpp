// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameState.h"
#include "TakahanoParis.h" 
#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Interfaces/SaveGameInterface.h"


bool ACustomGameState::SaveGame_Validate()
{
	return true;
}

void ACustomGameState::SaveGame_Implementation()
{
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
	// Save data
	TArray<AActor *> ActorsToSave;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass() , ActorsToSave);
	for(auto it:ActorsToSave)
	{
		const auto SaveActor =  Cast<ISaveGameInterface>(it);
		if(SaveActor)
			SaveActor->SaveToSaveGame(SaveGameInstance);	
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("CustomSaveSlot"), 0);
	UE_LOG(LogSaveGame, Display, TEXT("Game Saved on Server"));

	SaveGameInstance = nullptr;
}


bool ACustomGameState::LoadGame_Validate()
{
	return true;
}

void ACustomGameState::LoadGame_Implementation()
{
	// is this really necessary ?
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
	SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CustomGameSlot"), 0));
	if (!SaveGameInstance)
		return;
	// Read from save .
	// something = SaveGameInstance->Something

	UE_LOG(LogSaveGame, Display, TEXT("Game Loaded on Server"));

	// Get rid of pointer so it gets GCed ;)
	SaveGameInstance = nullptr;
}
