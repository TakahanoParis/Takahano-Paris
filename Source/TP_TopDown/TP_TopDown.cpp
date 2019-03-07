#include "TP_TopDown.h"

DEFINE_LOG_CATEGORY(LogTP_TopDown);

#define LOCTEXT_NAMESPACE "FTP_TopDown"

void FTP_TopDown::StartupModule()
{
	UE_LOG(LogTP_TopDown, Warning, TEXT("TopDown module has started!"));
}

void FTP_TopDown::ShutdownModule()
{
	UE_LOG(LogTP_TopDown, Warning, TEXT("TopDown module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_TopDown, TP_TopDown)
