// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "OptionMenuWidget.generated.h"

/**
 *  @class UOptionMenuWidget Base Class for the Option Menu
 */
UCLASS()
class TAKAHANOPARIS_API UOptionMenuWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UOptionMenuWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(meta = (BindWidget))
		class UCustomButton * BackButton;

	
    virtual void BindDelegates() override;

	
	
};
