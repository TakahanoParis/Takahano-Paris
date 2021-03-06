// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "MainHUDWidget.generated.h"

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

	UPROPERTY(meta = (BindWidget))
	class ULocationWidget * LocationWidget;

	UPROPERTY()
		bool bDrawLocationWidget;

	UFUNCTION()
		void SetLocationWidget();
	
};
