// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Hero.h"
#include "Cymie.generated.h"

/**
 * 
 */
UCLASS()
class ACymie : public AHero
{
	GENERATED_BODY()

public:
	bool CanRun() override;

	void Run() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Run")
		float RunDrain;

	float GetRunStaminaDrain() override {return RunDrain;}
	
	
	
};
