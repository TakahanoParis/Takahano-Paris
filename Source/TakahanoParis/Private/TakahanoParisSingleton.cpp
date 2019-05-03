// Fill out your copyright notice in the Description page of Project Settings.
/*
#include "TakahanoParisSingleton.h"
#if WITH_EDITOR
void UTakahanoParisSingleton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateInputBindingsInfo();
}

void UTakahanoParisSingleton::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UpdateInputBindingsInfo();
}

void UTakahanoParisSingleton::UpdateInputBindingsInfo()
{
	const auto Base = InputBindingInfos;
	InputBindingInfos = UTakahanoParisStatics::GetSettingsInputBindingInfos();
	uint8 BaseNum = Base.Num();
	for (uint8 t = 0; t< BaseNum; t++ )
	{
		if (InputBindingInfos.IsValidIndex(t))
		{
			InputBindingInfos[t].SetIcons(Base[t].GetIcons());
			InputBindingInfos[t].SetInfo(Base[t].GetInfo());
			InputBindingInfos[t].SetIsSituational(Base[t].GetIsSituational());
		}
	}
}
#endif

**/