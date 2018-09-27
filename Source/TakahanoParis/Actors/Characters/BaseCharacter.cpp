 // Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "UnrealNetwork.h"
//#include "Gameplay/AnimaPlayerState.h"

//////////////////////////////////////////////////////////////////////////
// ABaseCharacter

ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Only rotates along the Z axis  when the controller rotates. Let the rest just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;


}

//////////////////////////////////////////////////////////////////////////
// Input

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Hero Abilities
	PlayerInputComponent->BindAction("Attack",			IE_Pressed, this, &ABaseCharacter::Attack);
	//PlayerInputComponent->BindAction("FirstAbility",	IE_Pressed, this, &ABaseCharacter::Ability);


	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

}

#if 0
uint8 ABaseCharacter::I_GetTeam()
{
//	AAnimaPlayerState* APState = Cast<AAnimaPlayerState>(this->PlayerState);
// return APState->I_GetTeam();
	return 0;
}

void ABaseCharacter::I_SetTeam(uint8 NewTeam)
{
}
#endif // 0

void ABaseCharacter::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("Do Attack"));
}

bool ABaseCharacter::Ability(const uint8 &Number)
{
	UE_LOG(LogTemp, Display, TEXT("Do Ability number &d"), Number);
	return false;
}


void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::SetReady(bool NewReady)
{
	bIsReady = NewReady;
	UE_LOG(LogTemp, Warning, TEXT("SetReady Called (IsAuthority = %s)"), HasAuthority() ? TEXT("True") : TEXT("False"));
    if (Role == ROLE_AutonomousProxy)
	    Server_IsReady(bIsReady);
	OnRep_IsReady();
}

bool ABaseCharacter::GetIsReady() const
{
	return bIsReady;
}

void ABaseCharacter::OnRep_IsReady()
{
}

bool ABaseCharacter::Server_IsReady_Validate(bool ServerIsReady)
{
	//TODO: Add some Verification that server allows ready state !!!! 
	return true;
}
void ABaseCharacter::Server_IsReady_Implementation(bool ServerIsReady)
{
	bIsReady = ServerIsReady;
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ABaseCharacter, bIsReady);
	DOREPLIFETIME_CONDITION(ABaseCharacter, bIsReady, COND_SkipOwner);

}