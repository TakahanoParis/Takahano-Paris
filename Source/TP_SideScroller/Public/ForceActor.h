// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(ForceActionDelegate);

class UBoxComponent;
class UPaperSprite;
/**
 *  A static mesh that player can climb
 */
UCLASS(BlueprintType)
class AForceActor : public AActor
{
	GENERATED_BODY()
public:

	AForceActor(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
		ForceActionDelegate PushForward;
	UPROPERTY()
		ForceActionDelegate PushBack;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsUpDown;

	UPROPERTY()
		UPaperSprite* DirectionSprite[2];



};
