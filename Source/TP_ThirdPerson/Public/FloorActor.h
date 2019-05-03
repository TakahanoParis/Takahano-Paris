// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FloorActor.generated.h"

class AHackCameraActor;

/**
 * 
 */
UCLASS(hideCategories = (Rendering, Replication, Input, Actor, Base, Collision))
class TP_THIRDPERSON_API AFloorActor : public ATriggerBox
{
	GENERATED_BODY()

public :

	AFloorActor();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floor")
		uint8 FloorIndex;

protected:
	virtual void OnFloorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	virtual void OnFloorLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = "Floor")
	void Setup();



private:

	UPROPERTY()
	TArray<AHackCameraActor*> Cameras;

public:
	UFUNCTION(BlueprintPure, Category = "Floor")
		FORCEINLINE uint8 GetFloorIndex() const { return FloorIndex; }

	UFUNCTION(BlueprintPure, Category = "Floor")
		FORCEINLINE TArray<AHackCameraActor*> GetCameras() const { return Cameras; }


	UFUNCTION(BlueprintPure, Category = "Floor", meta = (WorldContext = "WorldContextObject"))
		static TArray<AHackCameraActor*> GetCamerasInFloor(const UObject * WorldContextObject, uint8 floorID);

};
