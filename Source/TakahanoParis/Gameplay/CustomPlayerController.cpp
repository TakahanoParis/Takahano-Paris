// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Gameplay/CustomHUD.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
#include "UserInterface/CustomWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameMode.h"
#include "UnrealNetwork.h"
//#include "Components/PrimitiveComponent.h"

ACustomPlayerController::ACustomPlayerController(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	bAttachToPawn = true;
	bFindCameraComponentWhenViewTarget = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	if(!GetHUD())
	{
		auto GM = UGameplayStatics::GetGameMode(GetWorld());
		//if(GM)
			//ClientSetHUD(GM->)
	}

		
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
// "turn" handles devices that provide an absolute delta, such as a mouse.
// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", GetPawn(), &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACustomPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", GetPawn(), &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ACustomPlayerController::LookUpAtRate);
}

void ACustomPlayerController::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACustomPlayerController::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACustomPlayerController::Reset()
{
	Super::Reset();

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

bool ACustomPlayerController::GetActorInCylinderScreenByChannel(TArray<AActor *> &ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, ECollisionChannel TraceChannel, bool bDrawDebug)
{
	TArray < FHitResult> HitResult;
	FVector ViewLocation;
	FVector ViewDirection;


	bool result = DeprojectScreenPositionToWorld(ScreenTraceLocation.X, ScreenTraceLocation.Y, ViewLocation, ViewDirection);
	ViewDirection = GetTransformComponent()->GetComponentToWorld().Rotator().Vector();

	if (!result)
		return false;

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	if (bDrawDebug)
	{
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	FCollisionShape Shape;
	Shape.MakeSphere(Radius);

	result = GetWorld()->SweepMultiByChannel(HitResult, ViewLocation, ViewLocation + Range * ViewDirection, FQuat(), TraceChannel, Shape, CollisionQueryParams);
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

bool ACustomPlayerController::GetActorInCylinderScreenForObjects(TArray<AActor*>& ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes , const TArray<AActor*>& ActorstoIgnore, bool bDrawDebug)
{
	TArray < FHitResult> HitResult;
	FVector ViewLocation;
	FVector ViewDirection;

	FCollisionObjectQueryParams ObjectQueryParam(ObjectTypes);

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	if(bDrawDebug)
	{
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}
	CollisionQueryParams.AddIgnoredActors(ActorstoIgnore);


	bool result = DeprojectScreenPositionToWorld(ScreenTraceLocation.X, ScreenTraceLocation.Y, ViewLocation, ViewDirection);
	ViewDirection = GetTransformComponent()->GetComponentToWorld().Rotator().Vector();

	if (!result)
		return false;

	FCollisionShape Shape;
	Shape.MakeSphere(Radius);

	result = GetWorld()->SweepMultiByObjectType
	(
		HitResult,
		ViewLocation,
		ViewLocation + Range * ViewDirection,
		FQuat(),
		ObjectQueryParam,
		Shape,
		CollisionQueryParams
	);

	if (result)
	{
		ActorsInCylinder.Empty();
		for (auto it : HitResult)
		{
			if (it.Actor.IsValid())
				ActorsInCylinder.Add(it.Actor.Get());
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

template<typename ClassFilter>
bool ACustomPlayerController::GetActorsInCenterOfScreen(TArray<ClassFilter *>& OutActors)
{
	OutActors.Empty();
	//const FVector2D ScreenCenter = GetScreenCenterCoordinates();
	if (!GetHUD())
		return false;
	auto CustomHUD = Cast<ACustomHUD>(GetHUD());
	if (CustomHUD)
	{
		CustomHUD->GetActorsInCenterofScreen<ClassFilter>(OutActors, CenterOfScreenSpan);
	}
	return OutActors.Num() > 0 ;
}

UCustomWidget* ACustomPlayerController::AddWidgetToScreen(TSubclassOf<UCustomWidget> ClassToSpawn,FVector2D AnchorPoint, int ZOrder)
{
	if (!IsValid(ClassToSpawn))
		return nullptr;
	auto NewWidget = CreateWidget<UCustomWidget>(this, ClassToSpawn);
	NewWidget->SetPositionInViewport(AnchorPoint, true);
	NewWidget->AddToPlayerScreen(ZOrder);
	return NewWidget;
}

void ACustomPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomPlayerController, InitialCharacter);
}

