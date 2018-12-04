// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomSaveGame.generated.h"

/**
 *
 */
USTRUCT()
struct FPlayerSave
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
		FVector PlayerLocation;

	UPROPERTY(SaveGame)
		FText PlayerName;


	//Constructor
	FPlayerSave()
	{
	}
};

/**
 *	@class UCustomSaveGame
 *	@brief default class for our save games
 */
UCLASS()
class TAKAHANOPARIS_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;

	UCustomSaveGame();
	
	
	
	
};
