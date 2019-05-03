// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectiveWidget.h"
#include "TakahanoParis.h"
#include "Gameplay/CustomPlayerController.h"
#include "LocationWidget.h"

UObjectiveWidget::UObjectiveWidget() : Super()
{

}

void UObjectiveWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	SetVisibility(bInIsEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UObjectiveWidget::SetObjectiveMessage(const FText & Objective, bool IsActive)
{
	if (IsActive  && ObjectiveMessage)
	{
		ObjectiveMessage->SetText(Objective);
		SetVisibility(ESlateVisibility::Visible);
	}
	else
		SetVisibility(ESlateVisibility::Hidden);
}
