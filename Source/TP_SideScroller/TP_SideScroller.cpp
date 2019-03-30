#include "TP_SideScroller.h"

DEFINE_LOG_CATEGORY(LogTP_SideScroller);

#define LOCTEXT_NAMESPACE "TP_SideScroller"

void FTP_SideScroller::StartupModule()
{
	UE_LOG(LogTP_SideScroller, Warning, TEXT("TP_SideScroller module has started!"));
}

void FTP_SideScroller::ShutdownModule()
{
	UE_LOG(LogTP_SideScroller, Warning, TEXT("TP_SideScroller module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_SideScroller, TP_SideScroller)