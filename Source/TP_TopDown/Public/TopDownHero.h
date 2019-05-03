// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownCharacter.h"
#include "TopDownHero.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TP_TOPDOWN_API ATopDownHero : public ATopDownCharacter
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
};
