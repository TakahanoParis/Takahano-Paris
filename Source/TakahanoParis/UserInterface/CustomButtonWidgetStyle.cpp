// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomButtonWidgetStyle.h"
#include "TextBlock.h"
#include "Button.h"
#include "CustomButton.h"
#include "StyleDefaults.h"


FCustomButtonStyle::FCustomButtonStyle()
	: Font()
	, ColorAndOpacity()
	, ShadowOffset(FVector2D::ZeroVector)
	, ShadowColorAndOpacity(FLinearColor::Black)
{
}

void FCustomButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
		OutBrushes.Add(&HighlightShape);
		OutBrushes.Add(&UnderlineBrush);
}

const FCustomButtonStyle& FCustomButtonStyle::GetDefault()
{
	static TSharedPtr< FCustomButtonStyle > Default;
	if (!Default.IsValid())
	{
		Default = MakeShareable(new FCustomButtonStyle());
		Default->Font = FStyleDefaults::GetFontInfo();
	}
	return *Default.Get();
}

const FName FCustomButtonStyle::TypeName(TEXT("FCustomButtonStyle"));



void UCustomButtonWidgetStyle::SetStyle(UCustomButton* Button, const FCustomButtonStyle& WidgetStyle)
{
	if (!Button)
		return;
	Button->GetLabelWidget()->SetFont(WidgetStyle.Font);
	Button->GetLabelWidget()->SetColorAndOpacity(WidgetStyle.ColorAndOpacity);
	Button->GetLabelWidget()->SetShadowOffset(WidgetStyle.ShadowOffset);
	Button->GetLabelWidget()->SetShadowColorAndOpacity(WidgetStyle.ShadowColorAndOpacity);
	Button->GetButtonWidget()->WidgetStyle = WidgetStyle;
}
