// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHUDWidget.h"
#include "Gameplay/CustomPlayerController.h"
#include "LocationWidget.h"

UMainHUDWidget::UMainHUDWidget() : Super()
{
}

void UMainHUDWidget::SetLocationWidget()
{
	FVector2D Direction;
	bDrawLocationWidget = false;
	const auto Controller = GetOwningPlayer();
	const auto PC = Cast<ACustomPlayerController>(Controller);

	if (!PC)
		return;

	if (!LocationWidget)
		return;

	const auto OtherPawn = PC->GetOtherPlayerPawn(0);
	if (!OtherPawn)
		return;


	bDrawLocationWidget = PC->Direction2DToActor(OtherPawn, Direction);
	float x, y;
	const auto cosTheta = FVector2D::DotProduct(FVector2D(1.f,0.f), Direction);
	const auto sinTheta = FVector2D::DotProduct(FVector2D(0.f,1.f), Direction);
	if (FMath::Abs(cosTheta) > FMath::Abs(sinTheta))
	{
		x = FMath::Sign(cosTheta);
		y = sinTheta / cosTheta;
	}
	else
	{
		if (sinTheta == 0)
			return;
		y = FMath::Sign(sinTheta);
		x = cosTheta / sinTheta;
	}

	const FVector2D AbsoluteWidgetLocation = FVector2D(x, y);
	LocationWidget->SetVisibility(bDrawLocationWidget? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	LocationWidget->SetPositionInViewport(AbsoluteWidgetLocation);

	// first get the angle between the 
	// if x  = 1 , y = tan
	// if y = 1 ,x = 1/tan

}
