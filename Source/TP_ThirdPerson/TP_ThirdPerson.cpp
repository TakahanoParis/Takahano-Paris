#include "TP_ThirdPerson.h"

DEFINE_LOG_CATEGORY(TP_ThirdPerson);

#define LOCTEXT_NAMESPACE "TP_ThirdPerson"

void FTP_ThirdPerson::StartupModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_ThirdPerson module has started!"));
}

void FTP_ThirdPerson::ShutdownModule()
{
	UE_LOG(TP_ThirdPerson, Warning, TEXT("TP_ThirdPerson module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTP_ThirdPerson, TP_ThirdPerson)