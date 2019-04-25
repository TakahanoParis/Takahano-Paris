 // Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/CustomPlayerState.h"
#include "CustomCharacterMovementComponent.h"
#include "Gameplay/CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "TakahanoParis.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"


 //////////////////////////////////////////////////////////////////////////
// AThirdPersonCharacter

AThirdPersonCharacter::AThirdPersonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Only rotates along the Z axis  when the controller rotates. Let the rest just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetMesh()->SetRelativeRotationExact(FRotator(0.f, 90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}

void AThirdPersonCharacter::OnConstruction(const FTransform& Transform)
{
	SetCharacter();
	Super::OnConstruction(Transform);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Jump, IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Jump, IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Sprint, IE_Pressed, this, &AThirdPersonCharacter::Run);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Sprint, IE_Released, this, &AThirdPersonCharacter::StopRunning);

	// Hero Abilities
	//PlayerInputComponent->BindAction("Attack",			IE_Pressed, this, &AThirdPersonCharacter::Attack);
	//PlayerInputComponent->BindAction("FirstAbility",	IE_Pressed, this, &AThirdPersonCharacter::Ability);


	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Forward, this, &AThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis(UTakahanoParisStatics::Right, this, &AThirdPersonCharacter::MoveRight);

}

void AThirdPersonCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	GetWorld()->GetAuthGameMode()->RestartPlayer(GetController());
	//Super::FellOutOfWorld(); -> we don't want to delete oneself
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	I_TakeDamage(DamageAmount, EventInstigator);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}


bool AThirdPersonCharacter::CanRun()
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	return aPS->GetStamina() >= 0 ;
}

void AThirdPersonCharacter::Run()
{
	if (!CanRun())
		return;
	UCustomCharacterMovementComponent *  CharacterMovementComponent  = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;
	CharacterMovementComponent->bIsRunning = true;
}

void AThirdPersonCharacter::StopRunning()
{
	UCustomCharacterMovementComponent *  CharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;
	CharacterMovementComponent->bIsRunning = false;
}

 bool AThirdPersonCharacter::SetCharacter()
 {
	 return false;
 }

 float AThirdPersonCharacter::GetRunStaminaDrain()
{
	return 0;
}


void AThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const auto Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(Rotation);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const auto Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		const FVector Direction = UKismetMathLibrary::GetRightVector(Rotation);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AThirdPersonCharacter, bIsReady);
	DOREPLIFETIME_CONDITION(AThirdPersonCharacter, bIsReady, COND_SkipOwner);
}