
#include "CustomGameMode.h"
#include "Actors/Characters/Hero.h"
#include "Gameplay/CustomPlayerController.h"
#include "Gameplay/CustomHUD.h"
//#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "CustomGameInstance.h"
#include "CustomGameState.h"

ACustomGameMode::ACustomGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                                DefaultPlayerTeamID(0),
                                                                                DefaultAITeamID(254)
{
	DefaultPawnClass = AHero::StaticClass();
	PlayerControllerClass = ACustomPlayerController::StaticClass();
	PlayerStateClass = ACustomPlayerState::StaticClass();
	HUDClass = ACustomHUD::StaticClass();
	PlayerControllerNum = 0;
	MaxPlayerNumber = 10;
	bCanStartMatch = false;
	GameStateClass = ACustomGameState::StaticClass();
}


void ACustomGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//InitializeHUDForPlayer(NewPlayer);
}

void ACustomGameMode::PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UE_LOG(LogNet, Warning, TEXT("Joining"));
	if (PlayerControllerNum >= MaxPlayerNumber)
	{
		ErrorMessage = FString(TEXT("Server Full, Cannot Join."));
#if !UE_BUILD_SHIPPING
		UE_LOG(LogTemp, Warning, TEXT("Serveur Full"));
#endif
	}
}

void ACustomGameMode::Logout(AController * Exiting)
{
	PlayerControllerNum--;
	Super::Logout(Exiting);
}

bool ACustomGameMode::ReadyToStartMatch_Implementation()
{
	if (bCanStartMatch)
	{
		ResumeGame();
		return  Super::ReadyToStartMatch_Implementation(); // Super::ReadyToStartMatch_Implementation();
	}
	else
	{
		PauseGame();
		return false;
	}
}

bool ACustomGameMode::EndGameToMainMenuMap()
{
	const auto aGM = Cast < UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!aGM)
		return false;
	aGM->ReturnToMainMenu();
	return true;
}

AActor * ACustomGameMode::ChoosePlayerStart_Implementation(AController * Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACustomGameMode::SetPlayerDefaults(APawn * PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
	const auto Hero = Cast<AHero>(PlayerPawn);
	if (!Hero)
		return;
	Hero->SetupCamera();
}

APawn * ACustomGameMode::SpawnDefaultPawnFor_Implementation(AController * NewPlayer, AActor * StartSpot)
{
	auto Pawn =  Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	const auto Hero = Cast<AHero>(Pawn);
	if (!Hero)
		return Pawn;
	Hero->SetupCamera();
	return Hero;
}

void ACustomGameMode::GetAllPlayerControllers(TArray<ACustomPlayerController *>& OutControllers)
{
	const auto players = GetNumPlayers();
	OutControllers.Empty();
	for (int32 count = 0; count < players; count++)
	{
		const auto playerController = Cast<ACustomPlayerController>(UGameplayStatics::UGameplayStatics::GetPlayerController((UObject*)GetWorld(), count));
		if (playerController)
			OutControllers.Add(playerController);
	}
}

// @todo Implements OpenLevel
void ACustomGameMode::RestartGameLevel()
{
	const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (aGI)
		aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(this)));
	
}

void ACustomGameMode::SetGameOver(FString &GameOverMessage, const TEnumAsByte<EGameOverEnum> &GameOverReason)
{
	PlayState = EPlayStateEnum::PSE_GameOver;
	GameOverMessage = TEXT("Game Over");
	switch (GameOverReason)
	{
	case EGameOverEnum::GOE_Defeat :
		RestartGameLevel();
		break;
	case EGameOverEnum::GOE_StopPlay :
		EndGameToMainMenuMap();
	default:
		break;
	}
	OnGameOver_BP();
}



void ACustomGameMode::LoadLastSave()
{
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		gameState->LoadGame();
	}
}

void ACustomGameMode::PauseGame()
{

}

void ACustomGameMode::ResumeGame()
{

}

void ACustomGameMode::SetCanStartMatch()
{
	bCanStartMatch = true;
}
