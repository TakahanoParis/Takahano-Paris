#include "HackCameraActor.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonCharacter.h"
#include "FloorActor.h"
#include "ThirdPersonJulia.h"

AHackCameraActor::AHackCameraActor() : Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	PrimaryActorTick.bCanEverTick = true;
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera View"));
	CameraView->SetupAttachment(RootComponent);
	CameraView->bAutoActivate = true;
	CameraView->bUsePawnControlRotation = true;
	//RootComponent = CameraView;
	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	CameraMesh->SetupAttachment(CameraView);

	const auto Cameras = AFloorActor::GetCamerasInFloor(this, FloorID);
	int Index;
	Cameras.Find(this, Index);
	FString Temp = FString("CCTV - ");
	Temp.AppendInt(Index);
	Temp.AppendChars(*FString(" - "), 3);
	Temp.AppendInt(FloorID);
	CameraName = FName(*Temp);
	AuthorizedClass.Emplace(AThirdPersonJulia::StaticClass());
	MinimumUseDistance = 7000;
}


void AHackCameraActor::Use_Implementation(AThirdPersonCharacter * PlayerCharacter, bool &bShouldLockMovement)
{
	if (!PlayerCharacter) return;
	if (CanUse(PlayerCharacter) && Cast<APlayerController>(PlayerCharacter->GetController()))
		SetViewTarget(Cast<APlayerController>(PlayerCharacter->GetController()));
}

bool AHackCameraActor::CanUse(AThirdPersonCharacter* UserCharacter)
{
	return Super::CanUse(UserCharacter) && UserCharacter->GetFloorIndex() == FloorID;
}

void AHackCameraActor::SetViewTarget(APlayerController* PlayerController)
{
	PlayerController->SetViewTargetWithBlend(this, BlendTime, VTBlend_EaseInOut, BlendExponent, false);
	SetOwner(PlayerController);
	PlayerController->SetControlRotation(this->GetActorRotation());

}

FName AHackCameraActor::GetUsableName() const
{
	return CameraName;
}
