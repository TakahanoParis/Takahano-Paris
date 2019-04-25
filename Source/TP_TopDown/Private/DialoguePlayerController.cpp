// Fill out your copyright notice in the Description page of Project Settings.

#include "DialoguePlayerController.h"
#include "TakahanoParisStatics.h"
#include "UserWidget.h"
#include "DialogueNPC.h"
#include "DialogueGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueUserWidget.h"
#include "TP_TopDown.h"
#include "Components/AudioComponent.h"
#include "WidgetBlueprintLibrary.h"


ADialoguePlayerController::ADialoguePlayerController() : Super() , bDialogAvailable(false)
{
}

void ADialoguePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(UTakahanoParisStatics::Use, IE_Pressed, this, &ADialoguePlayerController::Use);
}


void ADialoguePlayerController::Use()
{
	if (DialogueTarget && bDialogAvailable)
		Dialogue(DialogueTarget);
}

void ADialoguePlayerController::OnDialogueReady_Implementation(AActor* DialogNPC)
{
	const auto  npc = Cast<ADialogueNPC>(DialogNPC);
	if(!npc)
		return;

	bDialogAvailable = true;
	DialogueTarget = npc;
}

void ADialoguePlayerController::OnDialogueLost(AActor* DialogNPC)
{
	const auto  npc = Cast<ADialogueNPC>(DialogNPC);
	if (!npc)
		return;

	bDialogAvailable = false;
	DialogueTarget = nullptr;
}

void ADialoguePlayerController::NPCFinishedDialogue(ADialogueNPC* Target)
{
	const auto  DGM = Cast<ADialogueGameMode>(UGameplayStatics::GetGameMode(this));
	DGM->AddTalkedToNPC(Target);
	EndDialogue(Target);
	
}

void ADialoguePlayerController::Dialogue_Implementation(ADialogueNPC * &dialogueTarget)
{
	dialogueTarget = DialogueTarget;
	if(DialogueTarget && bDialogAvailable)
	{

		if(!CurrentDialogueWidget)
		{
			CurrentDialogueWidget = CreateDialogueWidget();
		}

		if (CurrentDialogueWidget && DialogueTarget->GetNPCDialogue())
		{
			CurrentDialogueWidget->AddToViewport(0);
			const auto Dialogue = DialogueTarget->GetNPCDialogue();
			CurrentDialogueWidget->ContructFromNativeCode(Dialogue);
			CurrentDialogueWidget->NPCActor = DialogueTarget;
			//FInputModeUIOnly InputMode;
			//InputMode.SetWidgetToFocus(CurrentDialogueWidget->TakeWidget());
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, CurrentDialogueWidget, EMouseLockMode::LockOnCapture);

			/*Dialogue->AssignPersistentOuter(UGameplayStatics::GetGameInstance(this));
			
			if(!Dialogue->Data.IsValidIndex(0))
			{
#if WITH_EDITOR
				UE_LOG(LogTP_TopDown, Warning, TEXT(" Dialogue contains no node ! "));
#endif	
				return;
			}
			//ToNPCReply(CurrentDialogueWidget, Dialogue->Data[0]);
			*/
		}

		// a vous les studios !
#if WITH_EDITOR
		else
			UE_LOG(LogTP_TopDown, Warning, TEXT("condition \"dialogue && DialogueTarget->GetNPCDialogue()\" not met "));
#endif
	}
}

void ADialoguePlayerController::EndDialogue_Implementation(ADialogueNPC * &talkedTo)
{
	if (DialogueTarget)
	{
		talkedTo = DialogueTarget;
		DialogueTarget = nullptr;
		bDialogAvailable = false;

		if (!CurrentDialogueWidget)
			return;
		CurrentDialogueWidget->RemoveFromParent();

		UWidgetBlueprintLibrary::SetInputMode_GameOnly((APlayerController*)this);
	}
}

UDialogue* ADialoguePlayerController::GetAvailableDialogue(bool& bIsValid) const
{
	bIsValid = false;
	if (!DialogueTarget)
		return nullptr;

	const auto diag = DialogueTarget->GetNPCDialogue();
	if(diag)
	{
		bIsValid = true;
		return diag;
	}
	return nullptr;
}

UDialogueUserWidget * ADialoguePlayerController::CreateDialogueWidget_Implementation()
{
	if (!DialogueTarget)
		return nullptr;
	const auto dialog = DialogueTarget->GetNPCDialogue();

	bool valid;
	const auto WidgetClass = UTakahanoParisStatics::GetDialogueWidgetClass(valid);

	if (!dialog || !valid)
		return nullptr;

	const auto widget =  CreateWidget(this, WidgetClass, TEXT("DialogName"));
	const auto dialogwidget = Cast<UDialogueUserWidget>(widget);
	if(dialogwidget)
		return dialogwidget;
	if(widget)
		widget->FinishDestroy();
	return nullptr;
}

void ADialoguePlayerController::ToNPCReply(UDialogue * Dialogue, UDialogueUserWidget* DialogueWidget, FDialogueNode Node)
{
	if (!DialogueWidget)
		return;
	DialogueWidget->RunEventsForNode(Node);
	const auto Nodes = Dialogue->GetNextNodes(Node);
	for (auto it : Nodes)
	{
		if (CurrentDialogueWidget->IsConditionsMetForNode(it))
		{
			// Display NPC Node :
			break;
		}
	}
}

void ADialoguePlayerController::DisplayNPCNode(UDialogue* Dialogue, UDialogueUserWidget* DialogueWidget,
	FDialogueNode Node)
{
	DialogueWidget->RunEventsForNode(Node);
	if (Node.Sound)
	{
		if (DialogueSound)
			DialogueSound->Stop();
		DialogueSound = UGameplayStatics::SpawnSound2D(this, Node.Sound);
		DialogueSound->Play(); // is it necessary ?
	}


}



