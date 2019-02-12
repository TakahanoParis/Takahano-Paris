// Fill out your copyright notice in the Description page of Project Settings.

#include "TPPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Gameplay/CustomHUD.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
#include "UserInterface/CustomWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/TPGameMode.h"
#include "UnrealNetwork.h"
#include "Gameplay/CustomGameState.h"
#include "Actors/Interfaces/TeamInterface.h"
#include "UserInterface/MainHUDWidget.h"
#include "CustomAIController.h"
#include "CustomPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Components/PrimitiveComponent.h"

ATPPlayerController::ATPPlayerController(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
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

void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &ATPPlayerController::Turn);
	InputComponent->BindAxis("TurnRate", this, &ATPPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &ATPPlayerController::LookUp);
	InputComponent->BindAxis("LookUpRate", this, &ATPPlayerController::LookUpAtRate);
}

void ATPPlayerController::TurnAtRate(float Rate)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPPlayerController::Turn(float Val)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerYawInput(Val);
}

void ATPPlayerController::LookUp(float Val)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerPitchInput(Val);
}

void ATPPlayerController::LookUpAtRate(float Rate)
{
	if (!GetPawn())
		return;
	// calculate delta for this frame from the rate information
	GetPawn()->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ATPPlayerController::GetVisibleActorsWithInterface(TArray<AActor*>& OutActors, const TSubclassOf<UInterface> Interface) const
{
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), Interface, OutActors);
	return GetVisibleActorsInArray(OutActors, this);
}

bool ATPPlayerController::GetVisibleActorsInArray(TArray<AActor*>& OutActors, const APlayerController* Player)
{
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(Player->GetPawn());
	FCollisionObjectQueryParams ObjectsParam = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	for (int32 id = OutActors.Num() - 1; id >= 0; --id)
	{
		const auto it = OutActors[id];
			
		FVector ViewLocation;
		FRotator ViewRotator;
		Player->GetPlayerViewPoint(ViewLocation, ViewRotator);
		const FVector ViewVector = UKismetMathLibrary::GetForwardVector(ViewRotator);
		FVector ToOtherActor = it->GetActorLocation() - ViewLocation;
		ToOtherActor.Normalize();

		// Forget about acotrs behind us :
		if (FVector::DotProduct(ToOtherActor,ViewVector) < 0)
		{
			OutActors.RemoveAt(id, 1, false);
			continue;
		}

		struct FHitResult OutHit;
		Params.AddIgnoredActor(it);
		
		Player->GetWorld()->LineTraceSingleByObjectType(OutHit,it->GetActorLocation(), ViewLocation, ObjectsParam, Params);
		if (OutHit.IsValidBlockingHit())
			OutActors.RemoveAt(id, 1, false);	
	}
	if(OutActors.Num()>0)
		OutActors.Shrink();

	return OutActors.Num() > 0;
}


void ATPPlayerController::Reset()
{
	Super::Reset();

}

void ATPPlayerController::PostInitProperties()
{
	Super::PostInitProperties();
}

void ATPPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	ShowMainHUD(true);
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set up team ID
	const auto aGM = Cast<ATPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (aGM)
		ATPPlayerController::Execute_I_Server_SetTeam(this, FTeam(aGM->GetDefaultPlayerTeamID()));
}


UCustomWidget* ATPPlayerController::AddWidgetToScreen(TSubclassOf<UCustomWidget> ClassToSpawn,FVector2D AnchorPoint, int ZOrder)
{
	if (!IsValid(ClassToSpawn))
		return nullptr;
	auto NewWidget = CreateWidget<UCustomWidget>(this, ClassToSpawn);
	NewWidget->SetPositionInViewport(AnchorPoint, true);
	NewWidget->AddToPlayerScreen(ZOrder);
	return NewWidget;
}

void ATPPlayerController::OnCharacterDie()
{
	Server_OnCharacterDie();
	OnCharacterDie_BP();
}


bool ATPPlayerController::Server_OnCharacterDie_Validate()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Died"));
	return true;
}


void ATPPlayerController::Server_OnCharacterDie_Implementation()
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

void ATPPlayerController::PlayCutScene_Implementation()
{
	ShowMainHUD(true);
}

void ATPPlayerController::ActorSaveDataSaved_Implementation(const FActorData &Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s saved "), *GetName());
}

bool ATPPlayerController::Direction2DToActor(const AActor* Actor, FVector2D& Out)const
{
	if (!Actor)
		return false;

	const auto PlayerLoc= ((AActor*)this)->GetActorLocation();
	//const auto WorldPos = Actor->GetActorLocation();
	const FVector V = Actor->GetActorLocation() - PlayerLoc;

	const float angleToActor = FVector::DotProduct(PlayerLoc, V);

	// compare to forward vector of controller
	if(angleToActor > 0)
	{
		Out = FVector2D(0.f, 0.f);
		return false;
	}
	const auto L = FVector2D(V.X, 0.f);
	const auto M = FVector2D(0.f, V.Y);

	const auto V2D = FVector2D(V.Y, V.Z);
	const float X = FVector2D::DotProduct(V2D, L); // gauche ou Droite
	const float Y = FVector2D::DotProduct(V2D, M); //haut ou bas

	Out.X = X;
	Out.Y = Y;
	Out.Normalize();
	return true;
}

bool ATPPlayerController::ShowMainHUD(bool visibility)
{
	if(!HUDWidget)
	{
		if (!HUDWidgetClass.Get())
			return false;
		HUDWidget = CreateWidget<UMainHUDWidget>(this, HUDWidgetClass, TEXT("Main HUD Widget"));
		if (!HUDWidget)
			return false;
		HUDWidget->AddToPlayerScreen();
	}
	HUDWidget->SetVisibility(visibility ? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	return (visibility && HUDWidget);
}

bool ATPPlayerController::SetMainHUD(UMainHUDWidget* HUD)
{
	if(HUD)
	{
		HUDWidget = HUD;
	}
	return false;
}

FTeam ATPPlayerController::I_GetTeam() const
{
	const auto aPS = Cast <ACustomPlayerState>(PlayerState);
	if (aPS)
		return aPS->I_GetTeam();
	return FTeam::NoTeam;
}

void ATPPlayerController::I_SetTeam(FTeam NewTeam)
{
	const auto aPS = Cast <ACustomPlayerState>(PlayerState);
	if (aPS)
		return aPS->I_SetTeam(NewTeam);
}

bool ATPPlayerController::Server_UpdatePlayersCharacterAlly_Validate()
{
	return true;
}

void ATPPlayerController::Server_UpdatePlayersCharacterAlly_Implementation()
{
	FriendlyPawns.Empty();
	TArray<AActor * > Pawns;
	UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), Pawns);
	for (auto it: Pawns)
	{
		const auto Pawn = Cast<APawn>(it);
		if (!Pawn)
			break;
		if (FTeam::GetAttitude(this, Pawn) == ETeamAttitudeEnum::TAE_Friendly)
		{
			FriendlyPawns.Add(Pawn);
		}
	}
}


void ATPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPPlayerController, InitialCharacter);
	DOREPLIFETIME(ATPPlayerController, FriendlyPawns);
}

