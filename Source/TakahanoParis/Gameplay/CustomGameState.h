// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomGameState.generated.h"

/**
 *	@class ACustomGameState
 *	@brief GameState for TakahanoParis. this store info about the current state of the level
 *	@todo Set to use a FName when saving, also allow changing slot
 */
UCLASS()
class TAKAHANOPARIS_API ACustomGameState : public AGameState
{
	GENERATED_BODY()

public:

	/**
	 *	@fn SaveGame
	 *	@brief makes all the necessary calls to save game
	 */
		FORCEINLINE void SaveGame() { Server_SaveGame(); }

	/**
	 *	@fn LoadGame
	 *	@brief makes all the necessary calls to Load game
	 */
		FORCEINLINE void LoadGame() { Server_LoadGame(); }

protected :

	/**
	 *	@fn Server_SaveGame
	 *	@brief Called on server, calls a multicast so every one saves the game
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SaveGame();

	/**
	 *	@fn Server_LoadGame
	 *	@brief Called on server, calls a multicast so every one Load the game
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_LoadGame();

	/**
	 *	@fn Multicast_SaveGame
	 *	@brief Called on every client (including server "client"), saves the game
	 */
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SaveGame();

	/**
	 *	@fn Multicast_LoadGame
	 *	@brief Called on every client (including server "client"), loads the game
	 */
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_LoadGame();

public :

	/**
	 *	@fn SaveGame_BP
	 *	@brief makes all the necessary calls to Save game
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Save", meta = (DisplayName = "Save Game" ))
		void SaveGame_BP() { SaveGame(); }

	/**
	 *	@fn LoadGame_BP
	 *	@brief makes all the necessary calls to Load game
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Save", meta = (DisplayName = "Load Game"))
		void LoadGame_BP() { LoadGame(); }

	/**
	 *	@fn GetActorsInSavedGame_BP
	 *	@brief Gets all the names of actors in Save game
	 *	@note for debug purposes
	 */
	UFUNCTION()
		TArray<FName> GetActorsInSavedGame() const;

	/**
	 *	@fn GetActorsInSavedGame_BP
	 *	@brief Gets all the names of actors in Save game
	 *	@note for debug purposes
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Save", meta = (DisplayName = "Get Actors in Saved Game"))
		FORCEINLINE TArray<FName>  GetActorsInSavedGame_BP() const {return GetActorsInSavedGame(); }


	/**
	 *	@property DefaultSaveGameName 
	 *	@brief defaultName for the save file
	 */
	static FString DefaultSaveGameName;
	
	
};
