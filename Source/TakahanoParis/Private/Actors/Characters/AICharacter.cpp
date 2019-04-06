// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "CustomCharacterMovementComponent.h"
//#include "Perception/AIPerceptionComponent.h"




AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)), AttackRate(1.f), AttackRange(100.f), LifePoints(100.f)
{
	AIControllerClass = ACustomAIController::StaticClass();

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	const auto AI = GetCustomAIController();
	if(AI && PatrolPath)
	{
		AI->SetPathActor(PatrolPath);
		//AI->StartPatrol();
	}
	AttackCoolDown = AttackRate;
}

void AAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (AttackCoolDown > 0)
	{
		AttackCoolDown -= DeltaSeconds;
	}
}

void AAICharacter::Attack_Implementation(AActor* Target)
{
	//const auto aAIC = GetCustomAIController();
	if (Target && AttackCoolDown <= 0)
	{
		AttackCoolDown = AttackRate;
		UGameplayStatics::ApplyDamage(Target, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}


void AAICharacter::StopAttack_Implementation(AActor* Target)
{

}


bool AAICharacter::I_TakeDamage(const float& DamageAmount, AActor* Instigator)
{
	LifePoints -= DamageAmount;
	return true;
}

void AAICharacter::Run_Implementation()
{
	UCustomCharacterMovementComponent *  CharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;
	CharacterMovementComponent->bIsRunning = true;
}

void AAICharacter::StopRunning_Implementation()
{
	UCustomCharacterMovementComponent *  CharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;
	CharacterMovementComponent->bIsRunning = false;
}

void AAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, PatrolPath);
	DOREPLIFETIME(AAICharacter, AttackCoolDown);


}
