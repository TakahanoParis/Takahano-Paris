// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackboardKeyType_ActorArray.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

const UBlackboardKeyType_ActorArray::FDataType UBlackboardKeyType_ActorArray::InvalidValue = UBlackboardKeyType_ActorArray::FDataType(TArray<AActor *>());

UBlackboardKeyType_ActorArray::UBlackboardKeyType_ActorArray(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValueSize = sizeof(InvalidValue);
	SupportedOp = EBlackboardKeyOperation::Basic;
}

TArray<AActor * >  UBlackboardKeyType_ActorArray::GetValue(const UBlackboardKeyType_ActorArray * KeyOb, const uint8 * RawData)
{
	return GetValueFromMemory<TArray<AActor * > >(RawData);
}

bool UBlackboardKeyType_ActorArray::SetValue(UBlackboardKeyType_ActorArray* KeyOb, uint8* RawData,	TArray<AActor*> Value)
{
	return SetValueInMemory<TArray<AActor*>>(RawData, Value);
}

TArray<AActor * > UBlackboardKeyType_ActorArray::GetBlackboardValueAsArray(UBTNode* NodeOwner, const FBlackboardKeySelector& Key)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? BlackboardComp->GetValue<UBlackboardKeyType_ActorArray>(Key.SelectedKeyName) : TArray<AActor * >();
}



void UBlackboardKeyType_ActorArray::SetBlackboardValueAsArray(UBTNode* NodeOwner, const FBlackboardKeySelector& Key, TArray<AActor*> Value)
{
	if (UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_ActorArray>(Key.SelectedKeyName, Value);
	}
}
