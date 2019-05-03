// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "TakahanoParisStatics.h"
#include "HelpWidget.generated.h"



class UTextBlock;

/**
 *	@class UHelpWidget
 *	@brief Simply say which
 */
UCLASS(Category = "Help")
class TAKAHANOPARIS_API UHelpWidget : public UCustomWidget
{
	GENERATED_BODY()

public:

	UHelpWidget();

	void SetIsEnabled(bool bInIsEnabled) override;

protected:
	UPROPERTY(meta = (BindWidget),BlueprintReadOnly)
		UTextBlock * HelpMessageText;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FInputBindingInfo> SituationalInfo;

public:

	UFUNCTION(BlueprintCallable)
		void SetActiveHelpMessageByBinding(const FName &BindingName, bool IsActive = true);

	UFUNCTION(BlueprintCallable)
		void SetActiveHelpMessage(const FText &Message, bool IsActive = true);

protected:

	UFUNCTION(BlueprintPure)
		FText GetHelpMessageForBinding(const FName &BindingName)  const;
};
