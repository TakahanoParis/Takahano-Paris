// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "TakahanoParisStatics.h"
#include "ObjectiveWidget.generated.h"



class UTextBlock;

/**
 *	@class UObjectiveWidget
 *	@brief Simply say which
 */
UCLASS(Category = "Help")
class TAKAHANOPARIS_API UObjectiveWidget : public UCustomWidget
{
	GENERATED_BODY()

public:

	UObjectiveWidget();

	void SetIsEnabled(bool bInIsEnabled) override;

protected:
	UPROPERTY(meta = (BindWidget),BlueprintReadOnly)
		UTextBlock * ObjectiveMessage;

public:

	UFUNCTION(BlueprintCallable)
		void SetObjectiveMessage(const FText &Objective, bool IsActive = true);

};
