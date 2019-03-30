// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ClimbableInterface.h"
#include "ClimbableActor.generated.h"

class UBoxComponent;

/**
 *  A static mesh that player can climb
 */
UCLASS()
class AClimbableActor : public AStaticMeshActor, public IClimbableInterface
{
	GENERATED_BODY()

public:
		AClimbableActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


private:
	/** ScenenComponent used to find climb location */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Climb", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ClimbTarget;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Climb", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* PlayerDetectionBox;

	UFUNCTION()
	void OnBoxOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	
};
