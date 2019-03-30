// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerHero.h"
#include "SideScrollerCymie.generated.h"

/**
 * 
 */
UCLASS()
class ASideScrollerCymie : public ASideScrollerHero
{
	GENERATED_BODY()

public:
	bool CanRun();

	void Run();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Run")
		float RunDrain;

	float GetRunStaminaDrain() {return RunDrain;}


protected:
	bool SetCharacter() override;
};
