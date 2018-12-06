 // Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
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


//////////////////////////////////////////////////////////////////////////
// ABaseCharacter

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bCanJump = false;
	// Only rotates along the Z axis  when the controller rotates. Let the rest just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABaseCharacter::Run);

	// Hero Abilities
	//PlayerInputComponent->BindAction("Attack",			IE_Pressed, this, &ABaseCharacter::Attack);
	//PlayerInputComponent->BindAction("FirstAbility",	IE_Pressed, this, &ABaseCharacter::Ability);


	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

}

void ABaseCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	GetWorld()->GetAuthGameMode()->RestartPlayer(GetController());
	//Super::FellOutOfWorld(); -> we don't want to delete oneself
}

float ABaseCharacter::I_GetLifePoints() const
{
	const auto aPS = Cast<ACustomPlayerState>(GetController()->PlayerState);
	if (aPS)
		return aPS->I_GetLifePoints();
	return -1.f;
}

bool ABaseCharacter::I_TakeDamage(const float& DamageAmount, AActor* Instigator)
{
	const auto aPS = Cast<ACustomPlayerState>(GetController()->PlayerState);
	if (aPS)
		return aPS->I_TakeDamage(DamageAmount,Instigator);
	return -1.f;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	I_TakeDamage(DamageAmount, EventInstigator);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}

bool ABaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && bCanJump;
}

FTeam ABaseCharacter::I_GetTeam() const
bool ABaseCharacter::CanRun()
{
	return false;
}

void ABaseCharacter::Run()
{
	if (!CanRun())
		return;
	UCustomCharacterMovementComponent *  CharacterMovementComponent  = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;
	CharacterMovementComponent->bIsRunning = true;
}



FTeam ABaseCharacter::I_GetTeam() const
{
	if(GetController())
		if(GetController()->PlayerState)
	{
		const auto aPS = Cast<ACustomPlayerState>(GetController()->PlayerState);
		if (aPS)
			return aPS->I_GetTeam();
	}
	return 0;
}


void ABaseCharacter::I_SetTeam(FTeam NewTeam)
{
	const auto aPS = Cast<ACustomPlayerState>(GetController()->PlayerState);
	if (aPS)
		return aPS->I_SetTeam(NewTeam);
}




bool ABaseCharacter::Ability(const uint8 &Number)
{
	UE_LOG(LogTemp, Display, TEXT("Do Ability number &d"), Number);
	return false;
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
	if(Controller == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Fuck you !"));
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
	DOREPLIFETIME_CONDITION(ABaseCharacter, bCanJump, COND_SkipOwner);
}