// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomHUD.h"
#include "Engine/Canvas.h"


ACustomHUD::ACustomHUD() :Super()
{

}

void ACustomHUD::DrawHUD()
{
    FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	GetActorsInCenterofScreen_Backend(ActorsInCenterofScreen);
	Super::DrawHUD();
}

void ACustomHUD::SetClassFilter(TSubclassOf<AActor> NewClassFilter)
{
	ClassFilter = NewClassFilter;
}

bool ACustomHUD::GetActorsInCenterofScreen_Backend(TArray<AActor*>& OutActors)
{
	OutActors.Empty();
	const FVector2D CenterOfScreen(Canvas->SizeX/2.f, Canvas->SizeY/2.f);
	GetActorsInSelectionRectangle<AActor>(CenterOfScreen - CenterSpan, CenterOfScreen + CenterSpan, OutActors, true, false); // Using default values for the rest
	return OutActors.Num() > 0;
}
