// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClimbableActor.h"
#include "ForceMovableBox.generated.h"

class UBoxComponent;

/**
 *  A static mesh that player can climb
 */
UCLASS(BlueprintType)
class AForceMovableBox : public AClimbableActor
{
	GENERATED_BODY()
	
};
