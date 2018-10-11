// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
//#include "Components/PrimitiveComponent.h"

ACustomPlayerController::ACustomPlayerController(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{

}

void ACustomPlayerController::SetupInputComponent()
{

}

FVector2D ACustomPlayerController::GetScreenCenterCoordinates()
{
	int32 X, Y;
	GetViewportSize(X, Y);
	return FVector2D(X/2.0, Y/2.0);
}

bool ACustomPlayerController::GetAbsoluteCursorCoordinate(FVector2D& Coordinate)
{
	int32 X, Y;
	GetViewportSize(X, Y);
	FVector2D ViewPortDimension(X, Y), CursorPos;
	bool IsIn = GetMousePosition(CursorPos.X, CursorPos.Y);
	Coordinate = CursorPos / ViewPortDimension;
	return IsIn;
}

USceneComponent* ACustomPlayerController::GetVisibleComponentUnderCursor()
{
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(TraceChannel, true, HitResult);
	if (HitResult.Component.IsValid())
		return HitResult.Component.Get();
	return nullptr;
}

USceneComponent* ACustomPlayerController::GetComponentByObjectUnderCursor()
{
	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery_MAX);
	GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult);
	if (HitResult.Component.IsValid())
		return HitResult.Component.Get();
	return nullptr;
}

AActor* ACustomPlayerController::GetActorUnderCursor()
{
	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery_MAX);
	GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult);
	if (HitResult.Actor.IsValid())
		return HitResult.Actor.Get();
	return nullptr;
}

bool ACustomPlayerController::GetActorInCylinderScreen(TArray<AActor *> &ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, ECollisionChannel TraceChannel)
{
	TArray < FHitResult> HitResult;
	FVector ViewLocation;
	FVector ViewDirection;


	bool result = DeprojectScreenPositionToWorld(ScreenTraceLocation.X, ScreenTraceLocation.Y, ViewLocation, ViewDirection);
	ViewDirection = GetTransformComponent()->GetComponentToWorld().Rotator().Vector();

	if (!result)
		return false;
	
	FCollisionShape Shape;
	Shape.MakeSphere(Radius);

	result = GetWorld()->SweepMultiByChannel(HitResult, ViewLocation, ViewLocation + Range * ViewDirection, FQuat(), TraceChannel, Shape);
	//result = GetWorld()->SweepMultiByObjectType()
	if(result)
	{
		for(auto it : HitResult)
		{
			if (it.Actor.IsValid())
			{
				ActorsInCylinder.Add(it.Actor.Get());
				UE_LOG(LogTemp, Warning, TEXT("it = &s"), *it.Actor.Get()->GetName())
			}

		}
		return true;
	}
	return false;
}


bool ACustomPlayerController::GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, const float MinRecentTime)
{
	// Thanks to Rama @ue4code for the tips :)
	//Empty any previous entries
	CurrentlyRenderedActors.Empty();

	//Iterate Over Actors
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->GetLastRenderTime() > MinRecentTime)
		{
			CurrentlyRenderedActors.Add(*Itr);
		}
	}
	if (CurrentlyRenderedActors.Num()>0)
	{
		return true;
	}
	return false;
}

