// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownPlayerController.h"
#include "Dialogue.h"
#include "DialoguePlayerController.generated.h"

class ADialogueNPC;
class UDialogueUserWidget;
//class UDialogue;
/**
 * 
 */
UCLASS()
class TP_TOPDOWN_API ADialoguePlayerController : public ATopDownPlayerController
{
	GENERATED_BODY()

public :
	ADialoguePlayerController();


	void SetupInputComponent() override;

	UFUNCTION()
		virtual void Use();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
		void OnDialogueReady(AActor * DialogNPC);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void OnDialogueLost(AActor * DialogNPC);

	UFUNCTION()
		void NPCFinishedDialogue(ADialogueNPC * Target);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
		bool bDialogAvailable;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
		void Dialogue(ADialogueNPC * &dialogueTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
		void EndDialogue(ADialogueNPC * &talkedTo);

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
		ADialogueNPC * DialogueTarget;

	UFUNCTION(BlueprintPure, Category = "Dialogue", meta = (HideSelfPin))
	UDialogue * GetAvailableDialogue(bool &bIsValid) const;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
		UDialogueUserWidget *  CreateDialogueWidget();

private:
	UPROPERTY()
		UDialogueUserWidget * CurrentDialogueWidget;

protected:

	UFUNCTION(BlueprintPure, Category = "Dialogue", meta = (HideSelfPin))
		FORCEINLINE UDialogueUserWidget * GetCurrentDialogueWidget() const { return CurrentDialogueWidget; }


private:

	// Attempt of doing every darn thing in C++

	void ToNPCReply(UDialogue * Dialogue, UDialogueUserWidget* DialogueWidget, FDialogueNode Node);

	void DisplayNPCNode(UDialogue * Dialogue, UDialogueUserWidget* DialogueWidget, FDialogueNode Node);

	class UAudioComponent * DialogueSound;
};
