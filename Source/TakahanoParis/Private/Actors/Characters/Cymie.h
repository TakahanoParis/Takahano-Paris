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
	/** Override of Use function to add interactables action	 */
	virtual void Use() override;
	
};
