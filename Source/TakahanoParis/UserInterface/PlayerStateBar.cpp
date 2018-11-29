// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStateBar.h"
#include "Gameplay/CustomPlayerState.h"
#include "ProgressBar.h"



float UPlayerStateBar::GetPlayerStaminaPercent() const
{
	const auto aPS = Cast<ACustomPlayerState>(GetOwningPlayerState());
	if (!aPS)
		return 0;
	return aPS->GetStamina() / aPS->GetMaxStamina();
}

void UPlayerStateBar::BindDelegates()
{
	StaminaBar->PercentDelegate.BindUFunction(this, TEXT("GetPlayerStaminaPercent"));
}
