// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameState.h"
#include "TakahanoParis.h" 
#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "SaveGameSystem.h"
#include "PlatformFeatures.h"


FString ACustomGameState::DefaultSaveGameName = TEXT("NativeCustomSaveGame");

bool ACustomGameState::Server_SaveGame_Validate()
{
	return true;
}

void ACustomGameState::Server_SaveGame_Implementation()
{
#if 0
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
	if (!SaveGameInstance)
	{
		UE_LOG(LogSaveGame, Error, TEXT("Could not create SaveGame object %s"));
		return;
	}
	SaveGameInstance->SaveAllActors(this);

	//SaveGameInstance->SaveTime = FDateTime::Now();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultSaveGameName, 0);
	UE_LOG(LogSaveGame, Display, TEXT("Game Saved on Server at %s"), *SaveGameInstance->SaveTime.ToString());
	SaveGameInstance = nullptr;
#endif //0
	Multicast_SaveGame();

}


bool ACustomGameState::Server_LoadGame_Validate()
{
	return true;
}

void ACustomGameState::Server_LoadGame_Implementation()
{
#if 0 
		auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveGameName, 0));
		if (!SaveGameInstance)
		{
			if (!IPlatformFeaturesModule::Get().GetSaveGameSystem())
				UE_LOG(LogSaveGame, Error, TEXT("Save gamesystem unreachable"));
			UE_LOG(LogSaveGame, Error, TEXT("Save Game does not exist"));
		}
		// Read from save .
		SaveGameInstance->LoadActors(this);

		UE_LOG(LogSaveGame, Warning, TEXT("Game Loaded on Server from save %s"), *SaveGameInstance->SaveTime.ToString());
		// Get rid of pointer so it gets GCed ;)
		SaveGameInstance = nullptr;
#endif //0
		Multicast_LoadGame();
}

bool ACustomGameState::Multicast_SaveGame_Validate()
{
	return true;
}
void ACustomGameState::Multicast_SaveGame_Implementation()
{
	/*
	if (Role == ROLE_Authority)
		return; // Server already saved his version
		*/
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
	if (!SaveGameInstance)
	{
		UE_LOG(LogSaveGame, Error, TEXT("Could not create SaveGame object %s"));
		return;
	}

	// Save data
	TArray<AActor *> ActorsToSave;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveableActorInterface::StaticClass(), ActorsToSave);
	for (auto it : ActorsToSave)
	{
		SaveGameInstance->AddActorToSave(it);

	}
	//SaveGameInstance->SaveTime = FDateTime::Now();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultSaveGameName, 0);
	FString IsServerLog = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogSaveGame, Display, TEXT("Game Saved on %s at %s"),*IsServerLog,  *SaveGameInstance->SaveTime.ToString());

	SaveGameInstance = nullptr;
}

bool ACustomGameState::Multicast_LoadGame_Validate()
{
	return true;
}
void ACustomGameState::Multicast_LoadGame_Implementation()
{
	/*
	if (Role == ROLE_Authority)
		return; // Server already loaded his version
*/
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveGameName, 0));
	if (!SaveGameInstance)
	{
		if (!IPlatformFeaturesModule::Get().GetSaveGameSystem())
			UE_LOG(LogSaveGame, Error, TEXT("Save gamesystem unreachable"));
		UE_LOG(LogSaveGame, Error, TEXT("Save Game does not exist"));
	}
	// Read from save .
	SaveGameInstance->LoadActors(this);

	UE_LOG(LogSaveGame, Warning, TEXT("Game Loaded on Server from save %s"), *SaveGameInstance->SaveTime.ToString());
	// Get rid of pointer so it gets GCed ;)
	SaveGameInstance = nullptr;
}

TArray<FName> ACustomGameState::GetActorsInSavedGame() const
{
	auto SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveGameName, 0));
	if (!SaveGameInstance)
	{
		if (!IPlatformFeaturesModule::Get().GetSaveGameSystem())
			UE_LOG(LogSaveGame, Error, TEXT("Save gamesystem unreachable"));
		UE_LOG(LogSaveGame, Error, TEXT("Save Game does not exist"));
	}
	TArray<FActorSaveData> Actors;
	SaveGameInstance->GetSavedActors(Actors);
	// Read from save .
	TArray<FName> ActorsName;
	for (auto it : Actors) { ActorsName.Add(it.ActorName); }
	//SaveGameInstance = nullptr;
	return ActorsName;
}
