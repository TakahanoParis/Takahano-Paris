// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/CustomWidget.h"
#include "PlayerStateBar.generated.h"

/**
 * 
 */
UCLASS()
class TAKAHANOPARIS_API UPlayerStateBar : public UCustomWidget
{
	GENERATED_BODY()
public:

protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar * StaminaBar;

	/**
	 *	@fn GetPlayerStaminaPercent()
	 *	@return the percent of stamina left to the player
	 */
	UFUNCTION()
		float GetPlayerStaminaPercent() const;

	virtual void BindDelegates() override;

};
