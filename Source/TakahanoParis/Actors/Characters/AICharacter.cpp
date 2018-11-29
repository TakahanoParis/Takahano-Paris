// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"


void AAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, PatrolPath);


}

AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
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

void AAICharacter::Attack_Implementation(AActor* Target)
{
}
