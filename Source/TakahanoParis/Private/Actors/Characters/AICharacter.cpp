// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
//#include "Perception/AIPerceptionComponent.h"




AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) , AttackRate(1.f), AttackRange(100.f)
{
	AIControllerClass = ACustomAIController::StaticClass();
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

bool AAICharacter::Attack(AActor* Target)
{
	//const auto aAIC = GetCustomAIController();
	if (Target && AttackCoolDown <= 0)
	{
		AttackCoolDown = AttackRate;
		UGameplayStatics::ApplyDamage(Target, AttackDamage, GetController(), this, UDamageType::StaticClass());
		Attack_BP(Target);
		return true;
	}
	return false;
}

void AAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, PatrolPath);
	DOREPLIFETIME(AAICharacter, AttackCoolDown);


}