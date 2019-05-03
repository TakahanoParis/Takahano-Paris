// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "TakahanoParisStatics.h"
#include "MainHUDWidget.generated.h"


class UTargetWidget;
class UHelpWidget;
class UObjectiveWidget;

/**
 *	@class UMainHUDWidget
 *	@brief Main class for the HUD
 */
UCLASS()
class TAKAHANOPARIS_API UMainHUDWidget : public UCustomWidget
{
	GENERATED_BODY()

public:

	UMainHUDWidget();

	virtual void OnWidgetRebuilt() override;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Help", meta = (DisplayName = "Help"))
		UHelpWidget * HelpWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Target", meta = (DisplayName = "Target"))
		UTargetWidget* TargetWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Objective", meta = (DisplayName = "Objective"))
		UObjectiveWidget* ObjectiveWidget;
	
	
	UFUNCTION(BlueprintCallable, Category = "Help")
		void SetHelpByBinding(const FName &BindingName, bool IsActive = false);

	UFUNCTION(BlueprintCallable, Category = "Help")
		void SetHelp(const FText &Message, bool IsActive = false);

	UFUNCTION(BlueprintCallable, Category = "Target")
		void SetTarget(const FName &TargetName, const AActor* Target, bool IsActive = false);

	UFUNCTION(BlueprintCallable, Category = "Objective")
		void SetObjective(const FText &Objective, bool IsActive = false);

};
