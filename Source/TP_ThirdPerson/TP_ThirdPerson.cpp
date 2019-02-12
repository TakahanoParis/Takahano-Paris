// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "TP_ThirdPerson.h"

DEFINE_LOG_CATEGORY(TP_ThirdPerson);

#define LOCTEXT_NAMESPACE "FTP_ThirdPerson"

void FTP_ThirdPerson::StartupModule()
{
	UE_LOG(Race, Warning, TEXT("TP_ThirdPerson module has started!"));
}

void FTP_ThirdPerson::ShutdownModule()
{
	UE_LOG(Race, Warning, TEXT("TP_ThirdPerson module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_ThirdPerson, TP_ThirdPerson)