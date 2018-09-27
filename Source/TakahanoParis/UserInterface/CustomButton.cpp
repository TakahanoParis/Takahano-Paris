// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Styling/SlateWidgetStyleAsset.h"


UCustomButton::UCustomButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BindDelegates();
}

void UCustomButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	//@TODO: Check this frequently qs it is subject to change
	UE_LOG(LogSlate, Display, TEXT("WidgetRebuilt"));
									
	if (!ButtonStyle) // This make no sens to do anything if nothing is set properly
	{
		Super::PostLoad();
		UE_LOG(LogSlate, Error, TEXT("Button Style not set."));
		return;
	}
	if(!ButtonWidget)
	{
		Super::PostLoad();
		UE_LOG(LogSlate, Error, TEXT("Button Widget not set."));
		return;
	}
	ButtonWidget->Style_DEPRECATED = ButtonStyle; // Maybe this won't work in future UE4 version

	const FButtonStyle* StylePtr = ButtonStyle->GetStyle<FButtonStyle>();
	if (StylePtr != nullptr)
	{
		ButtonWidget->WidgetStyle = *StylePtr;
	}
	if (!LabelWidget)
	{
		Super::PostLoad();
		UE_LOG(LogSlate, Error, TEXT("Label Widget not set."));
		return;
	}
	LabelWidget->SetText(Label);
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
