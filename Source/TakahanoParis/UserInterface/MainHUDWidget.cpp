// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHUDWidget.h"
#include "HelpWidget.h"
#include "TargetWidget.h"
#include "ObjectiveWidget.h"

UMainHUDWidget::UMainHUDWidget() : Super()
{
}

void UMainHUDWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (HelpWidget && TargetWidget && ObjectiveWidget)
	{
		HelpWidget->SetVisibility(ESlateVisibility::Hidden);
		TargetWidget->SetVisibility(ESlateVisibility::Hidden);
		ObjectiveWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UMainHUDWidget::SetHelpByBinding(const FName& BindingName, bool IsActive)
{
	if (HelpWidget)
	{
		HelpWidget->SetActiveHelpMessageByBinding(BindingName, IsActive);
		HelpWidget->SetIsEnabled(IsActive);
		HelpWidget->SetVisibility(IsActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::SetHelp(const FText& Message, bool IsActive)
{
	if (HelpWidget)
	{
		HelpWidget->SetActiveHelpMessage(Message, IsActive);
		HelpWidget->SetIsEnabled(IsActive);
		HelpWidget->SetVisibility(IsActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::SetTarget(const FName& TargetName, const AActor* Target, bool IsActive)
{
	if (!TargetWidget)
		return;
	TargetWidget->SetIsEnabled(IsActive);
	TargetWidget->SetVisibility(IsActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (IsActive)
		TargetWidget->SetTargetName(TargetName);
}

void UMainHUDWidget::SetObjective(const FText& Objective, bool IsActive)
{
	if (!ObjectiveWidget)
		return;
	ObjectiveWidget->SetIsEnabled(IsActive);
	ObjectiveWidget->SetVisibility(IsActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (IsActive)
		ObjectiveWidget->SetObjectiveMessage(Objective, IsActive);
}
