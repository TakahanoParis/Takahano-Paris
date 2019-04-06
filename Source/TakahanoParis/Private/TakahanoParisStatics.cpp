// Fill out your copyright notice in the Description page of Project Settings.

#include "TakahanoParisStatics.h"
#include "Gameplay/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameState.h"
#include "GameModeInterface.h"
#include "HackInterface.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Animation/AnimBlueprint.h"
#include "TakahanoParisSingleton.h"
#include "Components/SkeletalMeshComponent.h"


bool FCharacterStruct::SetSkeletalMesh(USkeletalMeshComponent* Comp) const
{
	if (!Comp || !Mesh || !AnimInstance)
		return false;
	Comp->SetSkeletalMesh(Mesh);
	Comp->SetAnimInstanceClass(AnimInstance);
	return true;

}


bool UTakahanoParisStatics::EndGameToMainMenuMap(const UObject* WorldContextObject)
{
	const auto aGI = Cast < UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!aGI)
		return false;
	aGI->ReturnToMainMenu();
	return true;
}

 void UTakahanoParisStatics::GetAllPlayerControllers(TArray<class ACustomPlayerController*> &OutControllers, const UObject* WorldContextObject)
{
	const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (aGI)
		aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(WorldContextObject)));
}

 void UTakahanoParisStatics::RestartGameLevel( const UObject* WorldContextObject)
 {
	 const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	 if (aGI)
		 aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(WorldContextObject)));

 }

EPlayStateEnum UTakahanoParisStatics::SetGameOver(const UObject* WorldContextObject, FString& GameOverMessage,
	const EGameOverEnum& GameOverReason)
{
	//UGameplayStatics::GetGameMode(WorldContextObject);
	 	GameOverMessage = TEXT("Game Over");
	switch (GameOverReason)
	{
	case EGameOverEnum::GOE_Defeat:
		RestartGameLevel(WorldContextObject);
		break;
	case EGameOverEnum::GOE_StopPlay:
		EndGameToMainMenuMap(WorldContextObject);
	default:
		break;
	}
	return EPlayStateEnum::PSE_GameOver;
}

void UTakahanoParisStatics::LoadLastSave(const UObject* WorldContextObject)
{
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	if (gameState)
	{
		gameState->LoadGame();
	}
}

uint8 UTakahanoParisStatics::GetDefaultAITeamID(const UObject* WorldContextObject)
{
	const auto aGM = UGameplayStatics::GetGameMode(WorldContextObject);
	if (aGM)
		return  IGameModeInterface::Execute_GetDefaultAITeamID(aGM);
	return 1;	
}

uint8 UTakahanoParisStatics::GetDefaultPlayerTeamID(const UObject* WorldContextObject)
{
	const auto aGM = UGameplayStatics::GetGameMode(WorldContextObject);
	if (aGM)
		return  IGameModeInterface::Execute_GetDefaultPlayerTeamID(aGM);
	return 255;
}

bool UTakahanoParisStatics::CallHackInterfaceOnActor(AActor * Target, class AController * Instigator)
{
	const auto AsInterface = Cast<IHackInterface>(Target);
	if (!AsInterface)
		return false;
	if (Instigator)
		IHackInterface::Execute_I_Server_Hack(Target, Instigator);
	return true;
}

bool UTakahanoParisStatics::CallInteractInterfaceOnActor(AActor* Target, AController* Instigator)
{
	const auto AsInterface = Cast<IInteractInterface>(Target);
	if (!AsInterface)
		return false;
	if (Instigator)
		IHackInterface::Execute_I_Server_Use(Target, Instigator);
	return true;
}

UTakahanoParisSingleton* UTakahanoParisStatics::GetTakahanoParisData(bool& DataIsValid)
{
	DataIsValid = false;
	UTakahanoParisSingleton* DataInstance = Cast<UTakahanoParisSingleton>(GEngine->GameSingleton);
	if (!DataInstance) return nullptr;
	if (!DataInstance->IsValidLowLevel()) return nullptr;
	DataIsValid = true;
	return DataInstance;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisCymie(bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	return Singleton->Cymie;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisJulia(bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	return Singleton->Julia;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisCharacterByName(const FName& CharName, bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	IsValid = false;
	for(auto it : Singleton->OtherCharacters)
	{
		if (it.CharaterName == CharName)
		{
			IsValid = true;
			return it;
		}
	}
	return FCharacterStruct();
}

void UTakahanoParisStatics::SetTakahanoParisCharacter(const FCharacterStruct& Char, USkeletalMeshComponent * Comp)
{
	Char.SetSkeletalMesh(Comp);
}

