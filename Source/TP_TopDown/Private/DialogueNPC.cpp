// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNPC.h"
#include "DialoguePlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ADialogueNPC::ADialogueNPC() : Super(), bCanTalk(true), BoxExtent(300.f, 300.f, 200.f), MinimumCharactersInZone(2), EndMainDialogueLine(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if(GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -1 * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	}
	
	DialogueTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DialogueTriggerBox"));
	if (DialogueTrigger)
	{
		DialogueTrigger->SetupAttachment(RootComponent);
		DialogueTrigger->SetBoxExtent(BoxExtent, false);
		DialogueTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADialogueNPC::OnBoxBeginOverlap);
		DialogueTrigger->OnComponentEndOverlap.AddDynamic(this, &ADialogueNPC::OnBoxEndOverlap);
	}
}

// Called when the game starts or when spawned
void ADialogueNPC::BeginPlay()
{
	Super::BeginPlay();

	if(DialogueTrigger)
		DialogueTrigger->SetBoxExtent(BoxExtent, false);
}

// Called every frame
void ADialogueNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADialogueNPC::DialogueConditionFulfilled_Implementation()
{
	if ( PlayersInZone.Num() >= MinimumCharactersInZone && bCanTalk)
		return true;

	return false;
}

void ADialogueNPC::DialogueOccured(APlayerController * Player, bool bCannotTalkAgain)	{ 
	const auto PC = Cast<ADialoguePlayerController>(Player);
	if(PC)
	{
		PC->NPCFinishedDialogue(this);
		bCanTalk = false;
	}
}

void ADialogueNPC::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;
	const auto Pawn = Cast<ACharacter>(OtherActor);
	if (!Pawn)
		return;
	const auto PC = Cast<APlayerController>(Pawn->GetController());
	if (PC)
		OnPlayerEntered(PC);
}

void ADialogueNPC::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;
	const auto Pawn = Cast<ACharacter>(OtherActor);
	if (!Pawn)
		return;
	const auto PC = Cast<APlayerController>(Pawn->GetController());
	if (PC)
		OnPlayerLeft(PC);

}


void ADialogueNPC::OnPlayerEntered(APlayerController* Player)
{
	const auto Dialog = Cast<ADialoguePlayerController>(Player);
	if(!Dialog)
		return;

	const auto character  = Player->GetCharacter();
	if (character)
		PlayersInZone.Add(character);


	if(DialogueConditionFulfilled())
	{
		for (auto it : PlayersInZone)
		{
			const auto Controller = Cast<ADialoguePlayerController>(it->GetController());
			Controller->OnDialogueReady(this);
		}
	}
}

void ADialogueNPC::OnPlayerLeft(APlayerController* Player)
{
	const auto Dialog = Cast<ADialoguePlayerController>(Player);
	if (!Dialog)
		return;

	const auto character = Player->GetCharacter();
	if (character)
		PlayersInZone.Remove(character);

	if (!DialogueConditionFulfilled())
		Dialog->OnDialogueLost(this);
}
