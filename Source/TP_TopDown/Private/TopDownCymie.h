// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownHero.h"
#include "TopDownCymie.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ATopDownCymie : public ATopDownHero
{
	GENERATED_BODY()
	
public:
	bool SetCharacter() override;
	
	
};
