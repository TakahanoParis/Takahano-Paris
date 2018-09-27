// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "MainMenuWidget.generated.h"

/**
 *  @class UMainMenuWidget class
 *  Base Class for the Main Menu, Should be overridden in Blueprints to set the look.
 */
UCLASS()
class TAKAHANOPARIS_API UMainMenuWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);


protected:
	UPROPERTY(meta = (BindWidget))
		class UCustomButton * QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * HostButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * SettingsButton;
	
	UFUNCTION()
		void QuitButtonCliqued();

	UFUNCTION()
		void HostButtonCliqued();

	UFUNCTION()
		void JoinButtonCliqued();

	UFUNCTION()
		void SettingsButtonCliqued();


	virtual void BindDelegates() override;
	

private:
	class UCustomGameInstance * GetCustomGameInstance()const;
	
};
