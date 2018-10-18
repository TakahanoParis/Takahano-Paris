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
	FVector2D CenterOfScreen(Canvas->SizeX/2.f, Canvas->SizeY/2.f);
	GetActorsInSelectionRectangle<AActor>(CenterOfScreen - CenterSpan, CenterOfScreen + CenterSpan, OutActors, true, false); // Using default values for the rest
	UE_LOG(LogTemp, Warning, TEXT("There are : %d actors under middle of cursor"), OutActors.Num());
	return OutActors.Num() > 0;
}
