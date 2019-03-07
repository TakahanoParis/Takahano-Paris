// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ClimbableInterface.h"
#include "Engine/World.h"

class IClimbableInterface;

ASideScrollerCharacter::ASideScrollerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASideScrollerCharacter::OnRep_bCanJump()
{
	I_SetCanJump(bCanJump);
}



//////////////////////////////////////////////////////////////////////////
// Input

void ASideScrollerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASideScrollerCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASideScrollerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASideScrollerCharacter::TouchStopped);
}

void ASideScrollerCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ASideScrollerCharacter::MoveForward(float Value)
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void ASideScrollerCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASideScrollerCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ASideScrollerCharacter::OnCanClimb_Implementation(const AActor * Climbable)
{
}

bool ASideScrollerCharacter::Climb_Implementation(const AActor * Climbable)
{
	if (!Climbable)
		return false;
	const auto AsInterface = Cast<IClimbableInterface>(Climbable);
	if (!AsInterface)
		return false;
	const FVector Target = Climbable->GetActorTransform().TransformVector(AsInterface->GetClimbTopTarget());
	FHitResult result;
	GetWorld()->SweepSingleByChannel(result, Target, Target, GetActorQuat(), ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), FCollisionQueryParams::DefaultQueryParam);
	if (result.IsValidBlockingHit())
		return false;

	SetActorLocation(Climbable->GetActorTransform().TransformVector(Target), false, nullptr, ETeleportType::ResetPhysics);
	return true;
}

bool ASideScrollerCharacter::SetCanClimb_Validate(bool bNewCanClimb, AActor * NewClimbableActor) { return true; }
void ASideScrollerCharacter::SetCanClimb_Implementation(bool bNewCanClimb, AActor * NewClimbableActor)
{
	if (NewClimbableActor)
	{
		bCanClimb = bNewCanClimb;
		ClimbableActor = NewClimbableActor;
		return;
	}
	ClimbableActor = nullptr;
	bCanClimb = false;
}
void ASideScrollerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ABaseCharacter, bIsReady);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, bIsReady, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, bCanClimb, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, ClimbableActor, COND_SkipOwner);
}