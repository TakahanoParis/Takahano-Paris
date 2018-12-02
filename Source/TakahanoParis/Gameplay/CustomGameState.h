// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomGameState.generated.h"

/**
 *	@class ACustomGameState
 *	@brief GameState for TakahanoParis. this store info about the current state of the level
 */
UCLASS()
class TAKAHANOPARIS_API ACustomGameState : public AGameState
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable, WithValidation)
		void SaveGame();

	UFUNCTION(Server, Reliable, WithValidation)
		void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Save", meta = (DisplayName = "Save Game" ))
		void SaveGame_BP() { SaveGame(); }
	
	
	
	
};
