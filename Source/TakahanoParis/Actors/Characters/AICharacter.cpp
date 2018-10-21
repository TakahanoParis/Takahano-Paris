// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "UnrealNetwork.h"



void AAICharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, PatrolPath);


}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	const auto AI = GetCustomAIController();
	if(AI && PatrolPath)
	{
		AI->SetPathActor(PatrolPath);
		AI->StartPatrol();
	}
}
