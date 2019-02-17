#include "TP_TopDown.h"

DEFINE_LOG_CATEGORY(TP_ThirdPerson);

#define LOCTEXT_NAMESPACE "TP_TopDown"

void FTP_TopDown::StartupModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_TopDown module has started!"));
}

void FTP_TopDown::ShutdownModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_TopDown module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_TopDown, TP_TopDown)