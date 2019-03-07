// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Input/Reply.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"
#include "CustomWidget.generated.h"


/**
 * @class UCustomWidget class
 * describes the basic elements a widget can do. widgets are mostly menus and stuff, but buttons also inherit this class
 */
UCLASS(Abstract, ClassGroup = (Menu))
class TAKAHANOPARIS_API UCustomWidget : public UUserWidget
{
	GENERATED_BODY()

public:

UCustomWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**	Allows for setting Applying Variables set in Blueprint	*/
	virtual void OnWidgetRebuilt() override;
	

protected:

	virtual void PostLoad() override;

	UFUNCTION()
		virtual void BindDelegates();

	
	
	
	
};
