
#include "CustomGameMode.h"
#include "Actors/Characters/Hero.h"
//#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

ACustomGameMode::ACustomGameMode()
{

		DefaultPawnClass = AHero::StaticClass();
		PlayerControllerNum = 0;
		MaxPlayerNumber = 10;
		bCanStartMatch = false;
}

#if 0
FTeam ACustomGameMode::GetTeam(int TeamNumber)
{
	return  TeamLists.IsValidIndex(TeamNumber)? TeamLists[TeamNumber] : FTeam();
}

bool ACustomGameMode::TeamExists(int TeamNumber)
{
	return TeamLists.IsValidIndex(TeamNumber);
}
#endif // 0




void ACustomGameMode::PostLogin(APlayerController * NewPlayer)
{

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
		return  true; // Super::ReadyToStartMatch_Implementation();
	}
	else
	{
		PauseGame();
		return false;
	}
}

void ACustomGameMode::RestartGameLevel()
{

}
void ACustomGameMode::SetGameOver()
{
	PlayState = EPlayStateEnum::PSE_GameOver;
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
