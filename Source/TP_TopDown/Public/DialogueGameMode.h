// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGameMode.h"
#include "DialogueGameMode.generated.h"

class ADialogueNPC;

/**
 * 
 */
UCLASS()
class TP_TOPDOWN_API ADialogueGameMode : public ATopDownGameMode
{
	GENERATED_BODY()
public :
		ADialogueGameMode();

	void BeginPlay() override;
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
		TArray<ADialogueNPC * > NPCTalkedTo;

	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
		uint8 NPCNumToTalkTo;

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void AddTalkedToNPC(ADialogueNPC * NPC);


	UFUNCTION(BlueprintPure, Category = "Dialogue")
		bool HasTalkedToAllNPC() const { return NPCTalkedTo.Num() == NPCNumToTalkTo; }

	UFUNCTION(BlueprintPure, Category = "Dialogue")
		int	NumberOfNPCTalkedTo() const { return NPCTalkedTo.Num(); }


	


	
};