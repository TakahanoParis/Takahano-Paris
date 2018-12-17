// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"




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
}

void AAICharacter::Attack(AActor* Target)
{
	const auto aAIC = GetCustomAIController();
	if(Target)
	{
		Attack_BP(Target);
	}
}

void AAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, PatrolPath);


}