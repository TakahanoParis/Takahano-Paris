// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ObjectAndNameAsStringProxyArchive.h"
#include "CustomSaveGame.generated.h"


class AActor;



USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ActorClass;

	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
		FVector ActorLocation;

	UPROPERTY()
		FRotator ActorRotation;


	UPROPERTY()
	TArray<uint8> ActorData;

	friend FArchive& operator<<(FArchive& Ar, FActorSaveData& ActorData)
	{
		Ar << ActorData.ActorClass;
		Ar << ActorData.ActorName;
		Ar << ActorData.ActorTransform;
		Ar << ActorData.ActorData;
		return Ar;
	}
};


USTRUCT()
struct FSaveGameData
{
	GENERATED_BODY()

	UPROPERTY()
		FName GameID;

	UPROPERTY()
	FDateTime Timestamp;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	friend FArchive& operator<<(FArchive& Ar, FSaveGameData& GameData)
	{
		Ar << GameData.GameID;
		Ar << GameData.Timestamp;
		Ar << GameData.SavedActors;
		return Ar;
	}
};

struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true), in_inner_archive_(InInnerArchive)
	{
		ArIsSaveGame = true;
	}

	FSaveGameArchive& operator=(FSaveGameArchive& other)
	{
		if (this != &other)
		{
			in_inner_archive_ = other.in_inner_archive_;
			ArIsSaveGame = other.ArIsSaveGame;
		}
	
	}
	FArchive& in_inner_archive_;
};

/**
 *	@class UCustomSaveGame
 *	@brief default class for our save games
 *	from https://wiki.unrealengine.com/SaveGame_Pointers_and_Structs
 */
UCLASS()
class TAKAHANOPARIS_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()

public :
	UCustomSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic, SaveGame)
	FDateTime SaveTime;

protected:

	TArray<FActorSaveData> SavedActors;
public:

	UFUNCTION()
		void AddActorToSave(AActor* Actor);

protected:
	UPROPERTY()
		FSaveGameData SaveGameData;


		void CreateSaveGameData(FSaveGameArchive &BinaryData);
		void ReadFromBinary(TArray<uint8> &BinaryData);

public:
	UFUNCTION()
		void SaveAllActors(UObject * WorldContextObject);

	UFUNCTION()
		void LoadActors(UObject * WorldContextObject);

private:

	/**
	 *	@property BinaryData
	 *	@brief the chain of byte that stores SaveGameData
	 */
	UPROPERTY()
	TArray<uint8> BinaryData;

};
