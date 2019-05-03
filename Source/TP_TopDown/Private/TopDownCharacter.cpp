// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopDownCharacter.h"
#include "TP_TopDown.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Gameplay/CustomPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SkeletalMeshComponent.h"


ATopDownCharacter::ATopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm



	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotationExact(FRotator(0.f, -90.f, 0.f));
}

void ATopDownCharacter::OnConstruction(const FTransform& Transform)
{
	SetCharacter();
	Super::OnConstruction(Transform);
}


void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(UTakahanoParisStatics::Jump, IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Jump, IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction(UTakahanoParisStatics::Sprint, IE_Pressed, this, &ATopDownCharacter::Run);
	//PlayerInputComponent->BindAction(UTakahanoParisStatics::Sprint, IE_Released, this, &ATopDownCharacter::StopRunning);
	//PlayerInputComponent->BindAction(UTakahanoParisStatics::Sprint, IE_Pressed, this, &ATopDownCharacter::MoveToCursor);

	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Forward, this, &ATopDownCharacter::MoveForward);
	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Right, this, &ATopDownCharacter::MoveRight);

}


void ATopDownCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		Controller->StopMovement();
		const auto Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(Rotation);
		AddMovementInput(Direction, Value);
	}
}

void ATopDownCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		Controller->StopMovement();
		const auto Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = UKismetMathLibrary::GetRightVector(Rotation);
		AddMovementInput(Direction, Value);
	}
}

void ATopDownCharacter::Use_Implementation()
{
	UE_LOG(LogTP_TopDown, Display, TEXT("Use Function called on %s"), *GetName());
}


bool ATopDownCharacter::SetCharacter()
{
	return false;
}


