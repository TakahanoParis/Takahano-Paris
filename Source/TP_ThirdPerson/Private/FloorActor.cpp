// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorActor.h"
#include "Components/ShapeComponent.h"
#include "ThirdPersonCharacter.h"
#include "HackCameraActor.h"
#include "Kismet/GameplayStatics.h"


AFloorActor::AFloorActor() : Super(), FloorIndex(-1)
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this , &AFloorActor::OnFloorEnter);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AFloorActor::OnFloorLeave);
}

void AFloorActor::BeginPlay()
{
	Setup();
}

void AFloorActor::OnFloorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handles player character :
	const auto character = Cast<AThirdPersonCharacter>(OtherActor);
	if (character)
		character->FloorIndex = FloorIndex;
}

void AFloorActor::OnFloorLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto character = Cast<AThirdPersonCharacter>(OtherActor);
}

TArray<AHackCameraActor*> AFloorActor::GetCamerasInFloor(const UObject * WorldContextObject, uint8 floorID)
{
	TArray<AActor *> Actors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AFloorActor::StaticClass(), Actors);
	const auto floors = reinterpret_cast<TArray<AFloorActor*> &>(Actors);
	for (auto it: floors)
	{
		if (it->FloorIndex == floorID)
			return it->GetCameras();
	}
	return TArray<AHackCameraActor*>();
}

void AFloorActor::Setup_Implementation()
{
	Cameras.Empty();
	// Setup Cameras
	TArray<AActor*> InsideActors;
	GetCollisionComponent()->GetOverlappingActors(InsideActors);
	const TArray<AActor*> Cam =InsideActors.FilterByPredicate([](const AActor* const InActor)
	{
		return InActor->IsA(AHackCameraActor::StaticClass());
	});
	for (auto it : Cam)
		if (static_cast<AHackCameraActor*>(it))
		{
			Cameras.Add(static_cast<AHackCameraActor*>(it));
			static_cast<AHackCameraActor*>(it)->SetFloorIndex(FloorIndex);
		}


}
