// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonHero.h"
#include "ThirdPersonCymie.generated.h"

/**
 * 
 */
UCLASS()
class AThirdPersonCymie : public AThirdPersonHero
{
	GENERATED_BODY()

	AThirdPersonCymie();

public:
	bool CanRun() override;

	void Run() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Run")
		float RunDrain;

	float GetRunStaminaDrain() override {return RunDrain;}


protected:
	bool SetCharacter() override;
};
