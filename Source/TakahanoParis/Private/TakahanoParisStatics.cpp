// Fill out your copyright notice in the Description page of Project Settings.

#include "TakahanoParisStatics.h"
#include "Gameplay/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameState.h"
#include "GameModeInterface.h"


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
	const auto aGM = Cast<IGameModeInterface>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (aGM)
		return  aGM->GetDefaultAITeamID();
	return 1;	
}

uint8 UTakahanoParisStatics::GetDefaultPlayerTeamID(const UObject* WorldContextObject)
{
	const auto aGM = Cast<IGameModeInterface>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (aGM)
		return  aGM->GetDefaultPlayerTeamID();
	return 255;
}
