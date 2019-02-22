#include "TP_SideScroller.h"

DEFINE_LOG_CATEGORY(TP_SideScroller);

#define LOCTEXT_NAMESPACE "TP_SideScroller"

void FTP_SideScroller::StartupModule()
{
	UE_LOG(TP_SideScroller, Warning, TEXT("TP_SideScroller module has started!"));
}

void FTP_SideScroller::ShutdownModule()
{
	UE_LOG(TP_SideScroller, Warning, TEXT("TP_SideScroller module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_SideScroller, FTP_SideScroller)