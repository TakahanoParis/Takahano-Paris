// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "Components/Button.h"
#include "CustomButton.generated.h"

/**
 * @class UCustomButton
 * A button with an automatically added text Label
 */
UCLASS()
class TAKAHANOPARIS_API UCustomButton : public UCustomWidget
{
	GENERATED_BODY()
	
public:
	/**	Default Constructor */
    UCustomButton(const FObjectInitializer& ObjectInitializer);

	/**	Allows for setting Applying Variables set in Blueprint	*/
	virtual void OnWidgetRebuilt() override;

protected:
	/** The text of the Text box child, will eventually */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText Label;

protected:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock * LabelWidget;

	UPROPERTY(meta = (BindWidget))
		class UButton * ButtonWidget;

	/**	This is just a wrapper for a struct allowing to set the styles separately in a different Asset ( for reusability) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
		class USlateWidgetStyleAsset* ButtonStyle;

	UFUNCTION(BlueprintCallable, Category = "Button|Event")
		virtual void OnClicked();

	virtual void BindDelegates() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonClickedEvent	OnClickedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonPressedEvent	OnPressedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonReleasedEvent	OnReleaseDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent		OnHoverDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent		OnUnhoverDelegate;

	
};
