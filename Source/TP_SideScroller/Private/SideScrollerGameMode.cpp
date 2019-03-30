// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerGameMode.h"
#include "SideScrollerHero.h"
#include "Kismet/GameplayStatics.h"
#include "SideScrollerJulia.h"
#include "SideScrollerCymie.h"
//#include "UObject/ConstructorHelpers.h"

ASideScrollerGameMode::ASideScrollerGameMode()
{
	DefaultPawnClass = ASideScrollerHero::StaticClass();
}

UClass* ASideScrollerGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	HeroClasses.Emplace(ASideScrollerJulia::StaticClass());
	HeroClasses.Emplace(ASideScrollerCymie::StaticClass());

	APlayerController  * aPC = Cast<APlayerController>(InController);
	if (!aPC)
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	const auto PlayerID = UGameplayStatics::GetPlayerControllerID(aPC);

	UE_LOG(LogTemp, Warning, TEXT("Generating Pawn for Player %d"), PlayerID);
	if (HeroClasses.IsValidIndex(PlayerID))
		return HeroClasses[PlayerID];

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASideScrollerGameMode::BeginPlay()
{
	if (bIsSplitscreen)
		UGameplayStatics::CreatePlayer(this, 1);
}

