// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ObjectAndNameAsStringProxyArchive.h"
#include "CustomSaveGame.generated.h"


class AActor;



USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	FString ActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	FName ActorName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	FTransform ActorTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
		FVector ActorLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
		FRotator ActorRotation;

	UPROPERTY(SaveGame)
	TArray<uint8> ActorData;

	friend FArchive& operator<<(FArchive& Ar, FActorSaveData& ActorData)
	{
		Ar << ActorData.ActorClass;
		Ar << ActorData.ActorName;
		Ar << ActorData.ActorTransform;
		Ar << ActorData.ActorLocation;
		Ar << ActorData.ActorRotation;
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

	/**
	 *	@property SaveTime
	 *	@brief Time at wich this save has bv
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic, SaveGame)
	FDateTime SaveTime;

protected:

	/**
	 *	@property SavedActors
	 *	@brief Array of the data from actors
	 */
	TArray<FActorSaveData> SavedActors;
public:

	/**
	 *	@fn AddActorToSave
	 *	@brief Just add the actor to this save
	 *	@param Actor : The actor the to save
	 */
	UFUNCTION()
		void AddActorToSave(AActor* Actor);

protected:

	/**
	 *	@property SaveGameData
	 *	@brief The Actual save. You're welcome
	 */
	UPROPERTY()
		FSaveGameData SaveGameData;

	/**
	 *	@fn CreateSaveGameData
	 *	@brief Creates an Archive ready to be put in the save file
	 *	@param BinaryData : The archive you're building
	 */
	void CreateSaveGameData(FSaveGameArchive &BinaryData);

	/**
	 *	@fn ReadFromBinary
	 *	@brief Read a binary file (array of byte) and transform it into a Valid Archive to read info from
	 *	@param BinaryData : The binary you're reading
	 *	@note : Not used
	 */
	void ReadFromBinary(TArray<uint8> &BinaryData);

public:

	/**
	 *	@fn SaveAllActors
	 *	@brief Find all the actors in level marked for save and save them
	 *	@param WorldContextObject : a valid actor in the level
	 */
	UFUNCTION()
		void SaveAllActors(UObject * WorldContextObject);

	/**
	 *	@fn LoadActors
	 *	@brief Load Actors present in save, an apply their values to the instance present in level
	 *	@param WorldContextObject : a valid actor in the level
	 */
	UFUNCTION()
		void LoadActors(UObject * WorldContextObject);

	/**
	 *	@fn GetActorsInSavedGame
	 *	@brief Gets all the names of actors in Save game
	 *	@note for debug purposes
	 */
	UFUNCTION()
		void GetSavedActors(TArray<FActorSaveData> &ActorsArray);

private:

	/**
	 *	@property BinaryData
	 *	@brief the chain of byte that stores SaveGameData
	 */
	UPROPERTY()
	TArray<uint8> BinaryData;

};
