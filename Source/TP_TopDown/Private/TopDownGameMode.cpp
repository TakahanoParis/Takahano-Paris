// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"
#include "TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopDownGameMode::ATopDownGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}