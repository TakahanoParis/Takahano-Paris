// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomHUD.h"
#include "Engine/Canvas.h"


void ACustomHUD::DrawHUD()
{
	Super::DrawHUD();
	GetActorsInCenterofScreen_Backend(ActorsInCenterofScreen);
}

void ACustomHUD::GetActorsInCenterofScreen(TArray<AActor *> &OutActors, TSubclassOf<AActor> NewClassFilter, FVector2D NewCenterSpan)
{
	ClassFilter = NewClassFilter;
	CenterSpan = NewCenterSpan;
	OutActors = ActorsInCenterofScreen;
}

bool ACustomHUD::GetActorsInCenterofScreen_Backend(TArray<AActor*>& OutActors)
{
	FVector2D CenterOfScreen(Canvas->SizeX/2.f, Canvas->SizeY/2.f);
	GetActorsInSelectionRectangle(ClassFilter, CenterOfScreen - CenterSpan, CenterOfScreen + CenterSpan, OutActors); // Using default values for the rest
	return OutActors.Num() > 0;
}
