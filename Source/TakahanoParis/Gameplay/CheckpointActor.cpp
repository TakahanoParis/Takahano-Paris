// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckpointActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/CustomGameState.h"

ACheckpointActor::ACheckpointActor(const FObjectInitializer& ObjectInitializer)
{
	OnActorBeginOverlap.AddDynamic(this, &ACheckpointActor::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACheckpointActor::OnEndOverlap);
	OnCheckpointEntered.AddDynamic(this, &ACheckpointActor::OnPlayeredEntered);

}

void ACheckpointActor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s overlapped by %s begin"), *OverlappedActor->GetName(), *OtherActor->GetName());
	ActorsInside.Add(OtherActor);
	const auto hero = Cast<ACharacter>(OtherActor);
	if (!hero)
		return;

	auto APC = Cast<APlayerController>(hero->GetController());
	if (!APC)
		return;
	if(!PlayerPassed.Contains(APC))
	{
		PlayerPassed.Add(APC);
		OnCheckpointEntered.Broadcast();
	}
}

void ACheckpointActor::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s overlapped by %s end"), *OverlappedActor->GetName(), *OtherActor->GetName());
	ActorsInside.Remove(OtherActor);
}

void ACheckpointActor::OnPlayeredEntered_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("On playered Entered called"))
	const auto gameModeBase = UGameplayStatics::GetGameMode(this);
	if (!gameModeBase)
		return;
	if(PlayerPassed.Num() >= gameModeBase->GetNumPlayers()) // Shouldn't be superior, but still
	{
		// Save Game :
		const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(this));
		if(gameState)
		{
			gameState->SaveGame();
		}
	}
}


