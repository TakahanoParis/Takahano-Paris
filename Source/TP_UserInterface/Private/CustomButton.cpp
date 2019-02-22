// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CustomButtonWidgetStyle.h"
#include "SlateWidgetStyleAsset.h"


UCustomButton::UCustomButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BindDelegates();
}

void UCustomButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (!ButtonWidget || !LabelWidget)
	{
		UE_LOG(LogSlate, Error, TEXT("Custom button not set."));
		return;	// do not do anything if there's no button or no Label
	}
	LabelWidget->SetText(Label);

	if (!CustomButtonStyle)
	return;

	const auto Style = CustomButtonStyle->GetStyle<FCustomButtonStyle>();
	if (Style)
		UCustomButtonWidgetStyle::SetStyle(this, *Style);
	else
		UE_LOG(LogSlate, Error, TEXT("Custom style not set."));
}


void UCustomButton::BindDelegates()
{
	if (ButtonWidget)
	{
		ButtonWidget->OnClicked.Clear();
		ButtonWidget->OnClicked.AddDynamic(this, &UCustomButton::OnClicked);
	}
}

void UCustomButton::OnClicked()
{
	OnClickedDelegate.Broadcast();
}
