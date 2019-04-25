// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerCharacter.h"
#include "TP_SideScroller.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ClimbableInterface.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

class IClimbableInterface;

ASideScrollerCharacter::ASideScrollerCharacter()
{

	static const float CharacterHeight = 96.0f;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, CharacterHeight);

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
	CameraBoom->RelativeRotation = FRotator(-5.f,185.f,0.f);

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
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)

	bCanMoveY = true;

	GetMesh()->SetRelativeLocation(FVector(0.f,0.f, -1 *CharacterHeight));
	GetMesh()->SetRelativeRotationExact(FRotator(0.f, -90.f, 0.f));
}

void ASideScrollerCharacter::OnConstruction(const FTransform& Transform)
{
	SetCharacter();
	Super::OnConstruction(Transform);
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
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Right, this, &ASideScrollerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Forward, this, &ASideScrollerCharacter::MoveForward);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Climb, IE_Pressed, this, &ASideScrollerCharacter::Climb);


	
	//disable touch inputs for touchscreen
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ASideScrollerCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ASideScrollerCharacter::TouchStopped);
}

void ASideScrollerCharacter::MoveRight(float Value)
{
	// add movement in that direction
		AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ASideScrollerCharacter::MoveForward(float Value)
{
	if (bCanMoveY)
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
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

void ASideScrollerCharacter::OnCanClimb_Implementation(AActor * Climbable)
{
	if (Climbable->Implements<UClimbableInterface>())
		ClimbableActor = Climbable;
}

void ASideScrollerCharacter::Climb()
{

	if (!GetCanClimb() || !ClimbableActor)
	{
#if WITH_EDITOR
		if (GetCanClimb())
			UE_LOG(LogTP_SideScroller, Error, TEXT("Climb : Climbable Actor not set"));
		if (ClimbableActor)
			UE_LOG(LogTP_SideScroller, Error, TEXT("Climb : GetCanClimb returned false"));
#endif //WITH_EDITOR
		return;
	}
	

	// prevent retriggering
	bCanClimb = false;


	const auto AsInterface = Cast<IClimbableInterface>(ClimbableActor);
	// Safety first

	if (!ClimbableActor || !AsInterface ) //!Climbable->Implements<UClimbableInterface>())
			return;

#if WITH_EDITOR
	UE_LOG(LogTP_SideScroller, Display, TEXT("%s about to climb on %s"), *GetName(), *ClimbableActor->GetName());
#endif //WITH_EDITOR

	const FVector Target = ClimbableActor->GetActorLocation() + IClimbableInterface::Execute_GetClimbTopTarget(ClimbableActor);

	// Testing teleportation target
	FHitResult result;
	GetWorld()->SweepSingleByChannel(result, Target, Target, GetActorQuat(), ECollisionChannel::ECC_Visibility, GetCapsuleComponent()->GetCollisionShape(), FCollisionQueryParams::DefaultQueryParam);
	if (result.IsValidBlockingHit())
	{
		UE_LOG(LogTP_SideScroller, Warning, TEXT("%s can't find place to climb on %s"), *GetName(), *ClimbableActor->GetName());
			return;
	}

	UE_LOG(LogTP_SideScroller, Display, TEXT("%s Climbing on %s"), *GetName(), *ClimbableActor->GetName());

	// teleporting
	const bool teleport = SetActorLocation(Target, false, nullptr, ETeleportType::ResetPhysics);
#if WITH_EDITOR
	if(!teleport)
		UE_LOG(LogTP_SideScroller, Error, TEXT("%s Failed to climb on %s"), *GetName(), *ClimbableActor->GetName());
#endif //WITH_EDITOR


	// Setting callbacks
	OnClimbAnimFinished.Clear();
	OnClimbAnimFinished.AddDynamic(this, &ASideScrollerCharacter::FinishClimb);
	StartClimbAnim.Broadcast();

	// Preventing any movement from player
	DisableInput(Cast<APlayerController>(GetController()));
	return;
}

void ASideScrollerCharacter::FinishClimb_Implementation(const AActor* Climbable)
{
	// You climbed successfully
	EnableInput(Cast<APlayerController>(GetController()));
	return;
}


bool ASideScrollerCharacter::Server_SetCanClimb_Validate(bool bNewCanClimb, AActor * NewClimbableActor) { return true; }
void ASideScrollerCharacter::Server_SetCanClimb_Implementation(bool bNewCanClimb, AActor * NewClimbableActor)
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


void ASideScrollerCharacter::SetCanClimb(bool bNewCanClimb, AActor* NewClimbableActor)
{
	if (Role == ROLE_AutonomousProxy)
		Server_SetCanClimb(bNewCanClimb, NewClimbableActor);
	else if (Role == ROLE_Authority)
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
}


void ASideScrollerCharacter::BroadCastFinishClimb()
{
	if(GetClimbableActor())
		OnClimbAnimFinished.Broadcast(GetClimbableActor());
}

void ASideScrollerCharacter::SetCanMoveY(bool bNewCanMoveY)
{
	if (Role == ROLE_AutonomousProxy)
		Server_SetCanMoveY(bNewCanMoveY);
	else if(Role == ROLE_Authority)
	{
		bCanMoveY = bNewCanMoveY;
	}
}

bool ASideScrollerCharacter::SetCharacter()
{
	return false;
}

bool ASideScrollerCharacter::Server_SetCanMoveY_Validate(bool bNewCanMoveY) { return true; }
void ASideScrollerCharacter::Server_SetCanMoveY_Implementation(bool bNewCanMoveY)
{
	bCanMoveY = bNewCanMoveY;
}




void ASideScrollerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ABaseCharacter, bIsReady);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, bIsReady, COND_None);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, bCanClimb, COND_None);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, ClimbableActor, COND_None);
	DOREPLIFETIME_CONDITION(ASideScrollerCharacter, bCanMoveY, COND_None);
}