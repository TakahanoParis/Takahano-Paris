// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopDownPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "TopDownCharacter.h"
#include "Engine/World.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::CardinalCross;
	bIsUsingGamepad = false;
	bResetGamepadDetectionAfterNoInput = true;
	GamepadTimeout = 5.f;

	bool DataIsValid;
	const auto Material = UTakahanoParisStatics::GetCursorToWorldMaterial(DataIsValid);
	if(DataIsValid && Material)
	{
		CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
		CursorToWorld->SetupAttachment(RootComponent);
		CursorToWorld->SetDecalMaterial(Material);
		CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
		CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	}
	// Create a decal in the world to show the cursor's location
	
}



void ATopDownPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bIsUsingGamepad)
	{
		bMoveToMouseCursor = false;
		bShowMouseCursor = false;
	}

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	if (IsLocalController() && bResetGamepadDetectionAfterNoInput && bIsUsingGamepad)
	{
		float now = GetWorld()->TimeSeconds;
		if (now > LastGamepadInputTime + GamepadTimeout)
		{
			bIsUsingGamepad = false;
		}
	}
}

void ATopDownPlayerController::
SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction(UTakahanoParisStatics::Select, IE_Pressed, this, &ATopDownPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction(UTakahanoParisStatics::Select, IE_Released, this, &ATopDownPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction(UTakahanoParisStatics::PauseMenu, IE_Pressed, this, &ATopDownPlayerController::PauseMenu);
}

void ATopDownPlayerController::PauseMenu()
{
	static bool toggle = false;
	if(toggle)
		UTakahanoParisStatics::OpenPauseMenu(this, this);
	else
		UTakahanoParisStatics::ClosePauseMenu(this, this);
	toggle = toggle ? false : true;
}

void ATopDownPlayerController::MoveToMouseCursor()
{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
}


void ATopDownPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATopDownPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ATopDownPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ATopDownPlayerController::CancelMoveToLocation()
{
	StopMovement();
}

bool ATopDownPlayerController::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	UpdateGamepad(bGamepad);
	return Super::InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
}

bool ATopDownPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	UpdateGamepad(bGamepad);
	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}

void ATopDownPlayerController::UpdateGamepad(const bool &bGamepad) const
{
	bIsUsingGamepad = bGamepad;
	if (bGamepad)
		LastGamepadInputTime = GetWorld()->TimeSeconds;
}

void ATopDownPlayerController::SetCursorLocation()
{
	if (!CursorToWorld)
		return;
	
	FHitResult TraceHitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	const FVector Location = TraceHitResult.Location;
	const FVector CursorFV = TraceHitResult.ImpactNormal;
	const FRotator CursorR = CursorFV.Rotation();
	CursorToWorld->SetWorldLocation(Location);
	CursorToWorld->SetWorldRotation(CursorR);
	CursorLocation = Location;
}

