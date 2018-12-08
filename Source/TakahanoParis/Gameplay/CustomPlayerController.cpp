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
#include "Gameplay/CustomGameState.h"
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
	InputComponent->BindAxis("Turn", this, &ACustomPlayerController::Turn);
	InputComponent->BindAxis("TurnRate", this, &ACustomPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &ACustomPlayerController::LookUp);
	InputComponent->BindAxis("LookUpRate", this, &ACustomPlayerController::LookUpAtRate);
}

void ACustomPlayerController::TurnAtRate(float Rate)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACustomPlayerController::Turn(float Val)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Val);
}

void ACustomPlayerController::LookUp(float Val)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerPitchInput(Val);
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

bool ACustomPlayerController::GetVisibleActorsInArray(TArray<AActor*>& OutActors, const APlayerController* Player)
{
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(Player->GetPawn());
	FCollisionObjectQueryParams ObjectsParam = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	OutActors.Shrink();
	for (int32 id = OutActors.Num() - 1; id >= 0; --id)
	{
		Params.AddIgnoredActor(OutActors[id]);
		struct FHitResult OutHit;
		Player->GetWorld()->LineTraceSingleByObjectType(OutHit, OutActors[id]->GetActorLocation(), Player->GetPawn()->GetActorLocation(), ObjectsParam, Params);
		if (OutHit.IsValidBlockingHit())
			OutActors.RemoveAt(id, 1, false);	
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


bool ACustomPlayerController::Server_OnCharacterDie_Validate()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Died"));
	return true;
}


void ACustomPlayerController::Server_OnCharacterDie_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Died"));
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(this));
	if(gameState)
	{
		PlayCutScene();
		gameState->LoadGame();
		// Load Game
	}
	
}

void ACustomPlayerController::OnCharacterDie_Implementation()
{
	Server_OnCharacterDie();
}

void ACustomPlayerController::PlayCutScene_Implementation()
{

}

void ACustomPlayerController::ActorSaveDataSaved_Implementation(const FActorData &Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s saved "), *GetName());
}

void ACustomPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomPlayerController, InitialCharacter);
}

