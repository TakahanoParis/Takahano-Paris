// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerGameMode.h"
#include "SideScrollerHero.h"
#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"

ASideScrollerGameMode::ASideScrollerGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	/*
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
	DefaultPawnClass = ASideScrollerHero::StaticClass();
}

UClass* ASideScrollerGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
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

