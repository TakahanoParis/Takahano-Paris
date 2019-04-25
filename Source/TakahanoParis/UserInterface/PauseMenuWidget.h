// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "PauseMenuWidget.generated.h"

/**	
 *  @class UPauseMenuWidget class
 *  Base Class for the Main Menu, but in game, Should be overridden in Blueprints to set the look.
 */
UCLASS()
class TAKAHANOPARIS_API UPauseMenuWidget : public UCustomWidget
{
	GENERATED_BODY()
public:
		virtual void SetIsEnabled(bool bInIsEnabled) override;

protected:
	UPROPERTY(meta = (BindWidget))
		class UCustomButton * QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * ReturnButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * NewGameButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * SettingsButton;

	UPROPERTY(meta = (BindWidget))
		class UCustomButton * BackToMainMenuButton;

	UFUNCTION()
		void QuitButtonCliqued();

	UFUNCTION(BlueprintNativeEvent) // QnD, @todo should revert to nothing
		void NewGameButtonCliqued();

	UFUNCTION(BlueprintNativeEvent)
		void BackToMainMenuButtonCliqued();

	UFUNCTION()
		void  ReturnButtonCliqued();

	UFUNCTION()
		void SettingsButtonCliqued();


	virtual void BindDelegates() override;

};
