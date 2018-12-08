// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHUDWidget.h"
#include "Gameplay/CustomPlayerController.h"

void UMainHUDWidget::SetLocationWidget()
{
	FVector2D Direction;
	const auto Controller = GetOwningPlayer();
	if(!Controller)
	if(LocationWidget)
	{
		const auto PC  = Cast<ACustomPlayerController>(Controller);
		const auto OtherPawn = PC->GetOtherPlayerPawn(0);
		if (OtherPawn)
			const auto Result = PC->Direction2DToActor(OtherPawn, Direction);
	}
}
