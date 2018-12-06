// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomSaveGame.h"
#include "Engine/World.h"
#include "TakahanoParis.h"
#include "GameFramework/Actor.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Interfaces/SaveableActorInterface.h"


UCustomSaveGame::UCustomSaveGame() : Super()
{
	SaveTime = FDateTime::Now();


}

void UCustomSaveGame::AddActorToSave(AActor* Actor)
{
	FActorSaveData ActorRecord;
	ActorRecord.ActorName = FName(*Actor->GetName());
	ActorRecord.ActorClass = Actor->GetClass()->GetPathName();
	ActorRecord.ActorTransform = Actor->GetTransform();

	ActorRecord.ActorLocation = Actor->GetActorLocation();
	ActorRecord.ActorRotation = Actor->GetActorRotation();


	FMemoryWriter MemoryWriter(ActorRecord.ActorData, true);
	FSaveGameArchive Ar(MemoryWriter);
	Actor->Serialize(Ar);
	CreateSaveGameData(Ar);

	SavedActors.Add(ActorRecord);
	ISaveableActorInterface::Execute_ActorSaveDataSaved(Actor);
}

void UCustomSaveGame::SaveAllActors(UObject * WorldContextObject)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, USaveableActorInterface::StaticClass(), Actors);
	for (auto Actor : Actors)
	{
		UE_LOG(LogSaveGame, Display, TEXT(" Saving : %s"), *Actor->GetName());
		AddActorToSave(Actor);
	}
}

void UCustomSaveGame::CreateSaveGameData(FSaveGameArchive &BinaryData)
{
	SaveGameData.GameID = "1234";
	SaveGameData.Timestamp = FDateTime::Now();
	SaveGameData.SavedActors = SavedActors;
		
	BinaryData << SaveGameData;
}

void UCustomSaveGame::ReadFromBinary(TArray<uint8> &BinaryData)
{
	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	FromBinary.Seek(0);

	FromBinary << SaveGameData;

	FromBinary.FlushCache();
	BinaryData.Empty();
	FromBinary.Close();
}

void UCustomSaveGame::LoadActors(UObject  * WorldContextObject)
{
	TArray<AActor*> WorldActors;
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, USaveableActorInterface::StaticClass(), WorldActors);
	TArray<FName> WorldActorsNames;
	for (auto it : WorldActors)
	{
		WorldActorsNames.Add(FName(*it->GetName()));
	}

	UE_LOG(LogSaveGame, Display, TEXT("Starting to load %d Actors"), SaveGameData.SavedActors.Num());

	
	for (FActorSaveData ActorRecord : SaveGameData.SavedActors)
	{
			FTransform ActorTransform = ActorRecord.ActorTransform;
			const FVector  ActorLocation = ActorRecord.ActorLocation;
			const FRotator  ActorRotation = ActorRecord.ActorRotation;

			FName ActorName = ActorRecord.ActorName;
			// two possibilities :
			// - this actors already exist, in that case, we just have to set it to the saved transform and characteristics
			// - this actor was spawn during gameplay, we need to spawn it
			//To avoid unnecessary burden we only spawn those that where not here.
			// we also need to destroy all actors that have been destroyed during gameplay : ie . that does not exist in the game save


			const bool AlreadySpawned = WorldActorsNames.Contains(ActorName);
			UE_LOG(LogSaveGame, Display, TEXT(" Loading %s , Already spawned: %d"), *ActorName.ToString(), AlreadySpawned);
			if (AlreadySpawned)
			{
				AActor * AlreadySpawnedActor = nullptr;
				// suboptimal , but somehow find would not compile (not recognizing TArray template parameter)
				for(auto it : WorldActors)
				{
					if (FName(*it->GetName()) == ActorName)
					{
						AlreadySpawnedActor = it;
						break;
					}
				}

				if(!AlreadySpawnedActor)
					continue;
				

				// Get the pointer of the actor
				AlreadySpawnedActor->SetActorTransform(ActorTransform);
				AlreadySpawnedActor->SetActorLocationAndRotation(ActorLocation, ActorRotation, false, nullptr, ETeleportType::TeleportPhysics);
				FMemoryReader MemoryReader(ActorRecord.ActorData, true);
				if(MemoryReader.IsError())
				{
					continue;
				}
				FSaveGameArchive Ar(MemoryReader);
				//AlreadySpawnedActor->Serialize(Ar); // get all the data from the archive
				ISaveableActorInterface::Execute_ActorSaveDataLoaded(AlreadySpawnedActor);

				WorldActorsNames.Remove(ActorName);
				
				continue; // next saved actor
			}
	
			// this actor did not exist  initially
			FVector SpawnPos = ActorTransform.GetLocation();
			FRotator SpawnRot = ActorTransform.Rotator();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Name = ActorRecord.ActorName;
					   
			UClass* SpawnClass = FindObject<UClass>(ANY_PACKAGE, *ActorRecord.ActorClass);
			if (SpawnClass)
			{
				AActor* NewActor = GWorld->SpawnActor(SpawnClass, &SpawnPos, &SpawnRot, SpawnParams);
				FMemoryReader MemoryReader(ActorRecord.ActorData, true);
				FSaveGameArchive Ar(MemoryReader);
				NewActor->Serialize(Ar);
				NewActor->SetActorTransform(ActorRecord.ActorTransform);
				ISaveableActorInterface::Execute_ActorSaveDataLoaded(NewActor);
			}
		}
	UE_LOG(LogSaveGame, Display, TEXT("finished to load Actors"));

}

