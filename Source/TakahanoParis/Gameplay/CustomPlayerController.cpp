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
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACustomPlayerController::LookUpAtRate(float Rate)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ACustomPlayerController::GetVisibleActorsWithInterface(TArray<AActor*>& OutActors, const TSubclassOf<UInterface> Interface) const
{
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), Interface, OutActors);
	return GetVisibleActorsInArray(OutActors, this);
}

bool ACustomPlayerController::GetVisibleActorsInArray(TArray<AActor*>& OutActors, const ACustomPlayerController* Player)
{
	const FCollisionQueryParams Params;
	const FCollisionResponseParams ResponseParam;
	for (auto it : OutActors)
	{
		struct FHitResult OutHit;
		if (!Player->GetWorld()->LineTraceSingleByChannel(OutHit, it->GetActorLocation(), Player->GetPawn()->GetActorLocation(), ECollisionChannel::ECC_Visibility, Params, ResponseParam))
			OutActors.Remove(it);
	}
	return OutActors.Num() > 0;
}


void ACustomPlayerController::Reset()
{
	Super::Reset();

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

