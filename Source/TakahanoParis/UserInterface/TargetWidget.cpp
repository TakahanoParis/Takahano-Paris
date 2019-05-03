// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetWidget.h"
#include "TextBlock.h"


void UTargetWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	SetVisibility(bInIsEnabled?ESlateVisibility::Visible: ESlateVisibility::Hidden);
}

void UTargetWidget::SetTargetName(const FName& TargetName)
{
	if(TargetNameText)
		TargetNameText->SetText(FText::FromName(TargetName));
}
