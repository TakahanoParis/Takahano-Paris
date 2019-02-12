
#include "TPGameMode.h"
#include "Actors/Characters/TPTPHero.h"
#include "Gameplay/TPPlayerController.h"
#include "Gameplay/CustomHUD.h"
//#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "CustomGameInstance.h"
#include "CustomGameState.h"

ATPGameMode::ATPGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                                DefaultPlayerTeamID(0),
                                                                                DefaultAITeamID(254)
{
	DefaultPawnClass = ATPTPHero::StaticClass();
	PlayerControllerClass = ATPPlayerController::StaticClass();
	PlayerStateClass = ACustomPlayerState::StaticClass();
	HUDClass = ACustomHUD::StaticClass();
	PlayerControllerNum = 0;
	MaxPlayerNumber = 10;
	bCanStartMatch = false;
	GameStateClass = ACustomGameState::StaticClass();
}


void ATPGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//InitializeHUDForPlayer(NewPlayer);
}

void ATPGameMode::PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
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

void ATPGameMode::Logout(AController * Exiting)
{
	PlayerControllerNum--;
	Super::Logout(Exiting);
}

bool ATPGameMode::ReadyToStartMatch_Implementation()
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

bool ATPGameMode::EndGameToMainMenuMap()
{
	const auto aGM = Cast < UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!aGM)
		return false;
	aGM->ReturnToMainMenu();
	return true;
}

AActor * ATPGameMode::ChoosePlayerStart_Implementation(AController * Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ATPGameMode::SetPlayerDefaults(APawn * PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
	const auto TPTPHero = Cast<ATPTPHero>(PlayerPawn);
	if (!TPTPHero)
		return;
	TPTPHero->SetupCamera(CameraMode);
}

APawn * ATPGameMode::SpawnDefaultPawnFor_Implementation(AController * NewPlayer, AActor * StartSpot)
{
	auto Pawn =  Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	const auto TPTPHero = Cast<ATPTPHero>(Pawn);
	if (!TPTPHero)
		return Pawn;
	TPTPHero->SetupCamera(CameraMode);
	return TPTPHero;
}

void ATPGameMode::GetAllPlayerControllers(TArray<ATPPlayerController *>& OutControllers)
{
	const auto players = GetNumPlayers();
	OutControllers.Empty();
	for (int32 count = 0; count < players; count++)
	{
		const auto playerController = Cast<ATPPlayerController>(UGameplayStatics::UGameplayStatics::GetPlayerController((UObject*)GetWorld(), count));
		if (playerController)
			OutControllers.Add(playerController);
	}
}

// @todo Implements OpenLevel
void ATPGameMode::RestartGameLevel()
{
	const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (aGI)
		aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(this)));
	
}

void ATPGameMode::SetGameOver(FString &GameOverMessage, const TEnumAsByte<EGameOverEnum> &GameOverReason)
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



void ATPGameMode::LoadLastSave()
{
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		gameState->LoadGame();
	}
}

void ATPGameMode::PauseGame()
{

}

void ATPGameMode::ResumeGame()
{

}

void ATPGameMode::SetCanStartMatch()
{
	bCanStartMatch = true;
}
