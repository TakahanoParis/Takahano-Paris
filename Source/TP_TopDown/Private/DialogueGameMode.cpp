// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueGameMode.h"
#include "DialoguePlayerController.h"
#include "DialogueNPC.h"
#include "Kismet/GameplayStatics.h"
#include "TP_TopDown.h"


ADialogueGameMode::ADialogueGameMode() : Super()
{
	PlayerControllerClass = ADialoguePlayerController::StaticClass();
}

void ADialogueGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllNPCs;
	UGameplayStatics::GetAllActorsOfClass(this, ADialogueNPC::StaticClass(), AllNPCs);
	NPCNumToTalkTo = AllNPCs.Num();
}

void ADialogueGameMode::AddTalkedToNPC(ADialogueNPC* NPC)
{

	if(NPC)
	{
		int index;
		if (!NPCTalkedTo.Find(NPC, index))
		{
			const auto newNPC = NPCTalkedTo.AddUnique(NPC);
#if WITH_EDITOR
			if (HasTalkedToAllNPC())
			UE_LOG(LogTP_TopDown, Warning, TEXT("Succesfully talked to all NPC !"));
#endif // WITH_EDITOR
		}




	}
}
