// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"
#include "TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownCymie.h"
#include "TopDownJulia.h"

ATopDownGameMode::ATopDownGameMode()
{
	HeroClasses.Empty();
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownPlayerController::StaticClass();
	DefaultPawnClass = ATopDownCharacter::StaticClass();
	HeroClasses.Emplace(ATopDownCymie::StaticClass());
	HeroClasses.Emplace(ATopDownJulia::StaticClass());
}


UClass* ATopDownGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
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

void ATopDownGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (bIsSplitscreen)
	{
		UGameplayStatics::CreatePlayer(this, 1, true);
		UTakahanoParisStatics::CreateMainHUDWidget(this);
	}
	
}
