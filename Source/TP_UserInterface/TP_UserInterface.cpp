#include "TP_UserInterface.h"

DEFINE_LOG_CATEGORY(TP_UserInterface);

#define LOCTEXT_NAMESPACE "TP_TopDown"

void FTP_UserInterface::StartupModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_TopDown module has started!"));
}

void FTP_UserInterface::ShutdownModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_TopDown module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_UserInterface, TP_UserInterface)