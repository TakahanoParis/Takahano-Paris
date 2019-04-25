// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "CustomButton.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameInstance.h"
#include "TakahanoParisStatics.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (QuitButton) 
	{
		BindDelegates();
	}
}

void UMainMenuWidget::QuitButtonCliqued()
{
	UTakahanoParisStatics::GetCustomGameInstance(GetOwningPlayer())->ShowQuitGameConfirmationMessage();
}

void UMainMenuWidget::HostButtonCliqued_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Button Has been Clicked"));
	UTakahanoParisStatics::GetCustomGameInstance(GetOwningPlayer())->RequestHostGame();
}

void UMainMenuWidget::JoinButtonCliqued_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinButton Has been Clicked"));
}

void UMainMenuWidget::SettingsButtonCliqued()
{
	UE_LOG(LogTemp, Warning, TEXT("SettingsButton Has been Clicked"));
}


void UMainMenuWidget::BindDelegates()
{
	Super::BindDelegates();
	if (QuitButton)
	{
		QuitButton->OnClickedDelegate.Clear();
		QuitButton->OnClickedDelegate.AddDynamic(this, &UMainMenuWidget::QuitButtonCliqued);
	}
	if (HostButton)
	{
		HostButton->OnClickedDelegate.Clear();
		HostButton->OnClickedDelegate.AddDynamic(this, &UMainMenuWidget::HostButtonCliqued);
	}
	if (JoinButton)
	{
		JoinButton->OnClickedDelegate.Clear();
		JoinButton->OnClickedDelegate.AddDynamic(this, &UMainMenuWidget::JoinButtonCliqued);
	}
	if (SettingsButton)
	{
		SettingsButton->OnClickedDelegate.Clear();
		SettingsButton->OnClickedDelegate.AddDynamic(this, &UMainMenuWidget::SettingsButtonCliqued);
	}
}

