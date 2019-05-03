
#include "ThirdPersonGameMode.h"
#include "GameFramework/Character.h"
//#include "Gameplay/CustomPlayerController.h"
#include "Gameplay/CustomHUD.h"
#include "Gameplay/CustomGameInstance.h"
#include "Gameplay/CustomGameState.h"
#include "Gameplay/CustomPlayerState.h"
#include "ThirdPersonHero.h"
//#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonCymie.h"
#include "ThirdPersonJulia.h"
#include "ThirdPersonPlayerController.h"


AThirdPersonGameMode::AThirdPersonGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                                DefaultPlayerTeamID(0),
                                                                                DefaultAITeamID(254)
{

	DefaultPawnClass = AThirdPersonHero::StaticClass();
	PlayerControllerClass = AThirdPersonPlayerController::StaticClass();
	PlayerStateClass = ACustomPlayerState::StaticClass();
	HUDClass = ACustomHUD::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();
	HeroClasses.Empty();
	HeroClasses.Emplace(AThirdPersonCymie::StaticClass());
	HeroClasses.Emplace(AThirdPersonJulia::StaticClass());
}

void AThirdPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
	UTakahanoParisStatics::CreateMainHUDWidget(this);
}


UClass* AThirdPersonGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	APlayerController  * aPC = Cast<APlayerController>(InController);
	if (!aPC)
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	const auto PlayerID = UGameplayStatics::GetPlayerControllerID(aPC);

	UE_LOG(LogTemp, Warning, TEXT("Generating Pawn for Player %d"), PlayerID);
	if (HeroClasses.IsValidIndex(PlayerID))
		return HeroClasses[PlayerID];

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

