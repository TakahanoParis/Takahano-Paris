// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidget.h"

UCustomWidget::UCustomWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BindDelegates();
}

void UCustomWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	BindDelegates(); // Somehow works when set here , at least it never fails to update :)
}

void UCustomWidget::PostLoad()
{
	Super::PostLoad();
	BindDelegates();
}

void UCustomWidget::BindDelegates()
{
}

