#include "PauseMenuWidget.h"
#include "PauseMenuWidget.h"
#include "PauseMenuWidget.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "CustomButton.h"
#include "Kismet/GameplayStatics.h"


void UPauseMenuWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	if(GetOwningPlayer())
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), bInIsEnabled);
}


void UPauseMenuWidget::QuitButtonCliqued()
{
	const auto GI = UTakahanoParisStatics::GetCustomGameInstance(GetOwningPlayer());
	if(GI)
		GI->ShowQuitGameConfirmationMessage();
}

void UPauseMenuWidget::BackToMainMenuButtonCliqued_Implementation()
{
	UTakahanoParisStatics::EndGameToMainMenuMap(GetOwningPlayer());
}

void UPauseMenuWidget::NewGameButtonCliqued_Implementation()
{

}

void UPauseMenuWidget::ReturnButtonCliqued()
{
	SetIsEnabled(false);
	RemoveFromParent();
}

void UPauseMenuWidget::SettingsButtonCliqued()
{
	UE_LOG(LogTemp, Warning, TEXT("SettingsButton Has been Clicked"));
}


void UPauseMenuWidget::BindDelegates()
{
	Super::BindDelegates();
	if (QuitButton)
	{
		QuitButton->OnClickedDelegate.Clear();
		QuitButton->OnClickedDelegate.AddDynamic(this, &UPauseMenuWidget::QuitButtonCliqued);
	}
	if (NewGameButton)
	{
		NewGameButton->OnClickedDelegate.Clear();
		NewGameButton->OnClickedDelegate.AddDynamic(this, &UPauseMenuWidget::NewGameButtonCliqued);
	}
	if (BackToMainMenuButton)
	{
		BackToMainMenuButton->OnClickedDelegate.Clear();
		BackToMainMenuButton->OnClickedDelegate.AddDynamic(this, &UPauseMenuWidget::BackToMainMenuButtonCliqued);
	}
	if (SettingsButton)
	{
		SettingsButton->OnClickedDelegate.Clear();
		SettingsButton->OnClickedDelegate.AddDynamic(this, &UPauseMenuWidget::SettingsButtonCliqued);
	}
	if (ReturnButton)
	{
		ReturnButton->OnClickedDelegate.Clear();
		ReturnButton->OnClickedDelegate.AddDynamic(this, &UPauseMenuWidget::ReturnButtonCliqued);
	}
	
}
