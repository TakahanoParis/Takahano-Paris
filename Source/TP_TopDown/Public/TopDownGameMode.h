// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "TopDownGameMode.generated.h"

class ATopDownCharacter;

UCLASS(minimalapi)
class ATopDownGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()

public:
	ATopDownGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Classes)
		TArray<TSubclassOf<ATopDownCharacter>> HeroClasses;

	/** @brief second player hero  */
	//TSubclassOf<ASideScrollerHero> Hero2Class;

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/** @brief is the game should be two players side by side ? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Multiplayer)
		bool bIsSplitscreen = true;

	void BeginPlay() override;

};



