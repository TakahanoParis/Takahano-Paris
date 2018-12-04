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

	UPROPERTY(meta = (BindWidget))
		class UProgressBar * HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UImage * PlayerImage;

	/**
	 *	@fn GetPlayerStaminaPercent()
	 *	@return the percent of stamina left to the player
	 */
	UFUNCTION()
		float GetPlayerStaminaPercent() const;

	/**
	 *	@fn GetPlayerHealthPercent()
	 *	@return the percent of health left to the player
	 */
	UFUNCTION()
		float GetPlayerHealthPercent() const;

	virtual void BindDelegates() override;

};
