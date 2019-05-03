// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "TargetWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(Category = "Target")
class TAKAHANOPARIS_API UTargetWidget : public UCustomWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		UTextBlock * TargetNameText;

public :
	virtual void SetIsEnabled(bool bInIsEnabled) override;

	UFUNCTION(BlueprintCallable)
		void SetTargetName(const FName &TargetName);
	
};
