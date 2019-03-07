// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "SideScrollerGameMode.generated.h"

class ASideScrollerHero;

UCLASS(minimalAPI)
class ASideScrollerGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()

public:
	ASideScrollerGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Classes)
	TArray<TSubclassOf<ASideScrollerHero>> HeroClasses;

	/** @brief second player hero  */
		//TSubclassOf<ASideScrollerHero> Hero2Class;


	/** @brief is the game should be two players side by side ? */
	UPROPERTY()
		bool bIsSplitscreen;

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

};



