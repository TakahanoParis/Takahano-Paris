// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "GameFramework/Actor.h"
#include "BlackboardKeyType_ActorArray.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Actor Array"))
class TAKAHANOPARIS_API UBlackboardKeyType_ActorArray : public UBlackboardKeyType
{
	GENERATED_UCLASS_BODY()

	typedef TArray<AActor * > FDataType;
	static const FDataType InvalidValue;

	static TArray<AActor * >  GetValue(const UBlackboardKeyType_ActorArray* KeyOb, const uint8* RawData);
	static bool SetValue(UBlackboardKeyType_ActorArray* KeyOb, uint8* RawData, TArray<AActor * >  Value);

	//virtual EBlackboardCompare::Type CompareValues(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock,
		//const UBlackboardKeyType* OtherKeyOb, const uint8* OtherMemoryBlock) const override;

	//virtual FString DescribeArithmeticParam(int32 IntValue, float FloatValue) const override;

protected:
	//virtual FString DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const override;
	//virtual bool TestArithmeticOperation(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock, EArithmeticKeyOperation::Type Op, int32 OtherIntValue, float OtherFloatValue) const override;
	
	UFUNCTION(BlueprintPure, Category = "AI|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static  TArray<AActor * > GetBlackboardValueAsArray(UBTNode* NodeOwner, const FBlackboardKeySelector& Key);

	UFUNCTION(BlueprintCallable, Category = "AI|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static void SetBlackboardValueAsArray(UBTNode* NodeOwner, const FBlackboardKeySelector& Key, TArray<AActor*> Value);


	
};
