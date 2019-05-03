// Fill out your copyright notice in the Description page of Project Settings.

#include "HelpWidget.h"
#include "TakahanoParis.h"
#include "Gameplay/CustomPlayerController.h"
#include "LocationWidget.h"
#include "TextBlock.h"

UHelpWidget::UHelpWidget() : Super()
{
	bool SingletonValid = false;
	/*
	const auto Base = UTakahanoParisStatics::GetAllInputBindingInfos(SingletonValid);
#if WITH_EDITOR
	if (!SingletonValid) UE_LOG(LogTakahanoParis, Error, TEXT("UMainHUDWidget : Singleton not valid, HUD wont have any help"));
#endif //WITH_EDITOR

	SituationalInfo = Base.FilterByPredicate([](const FInputBindingInfo &InputElem)
	{
		return InputElem.GetIsSituational();
	});
	*/
}

void UHelpWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	SetVisibility(bInIsEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UHelpWidget::SetActiveHelpMessageByBinding(const FName& BindingName, bool IsActive)
{
	if (IsActive  && HelpMessageText)
	{
		const auto Text = GetHelpMessageForBinding(BindingName);
		if(!Text.IsEmpty())
		{
			HelpMessageText->SetText(Text);
			SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
		SetVisibility(ESlateVisibility::Hidden);
}

void UHelpWidget::SetActiveHelpMessage(const FText& Message, bool IsActive)
{
	if (!Message.IsEmpty())
	{
		HelpMessageText->SetText(Message);
		SetVisibility(ESlateVisibility::Visible);
	}
	else
		SetVisibility(ESlateVisibility::Hidden);
}

FText UHelpWidget::GetHelpMessageForBinding(const FName& BindingName ) const
{
	const FInputBindingInfo * Binding =  SituationalInfo.FindByPredicate([BindingName](const FInputBindingInfo &InputElem)
	{
		return InputElem.GetName() == BindingName;
	}); 
	return Binding ? Binding->GetInfo() : FText();
}
