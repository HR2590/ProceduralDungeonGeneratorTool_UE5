// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonLibrary.generated.h"

class UPCGComponent;
/**
 * 
 */
UCLASS()
class LEVELGENERATOREDITOR_API UDungeonLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent,Category="PCG Parameters")
	void SetFloatParameter(const FName& ParameterName,const float& Value,UPCGComponent* InComp);
	UFUNCTION(BlueprintImplementableEvent,Category="PCG Parameters")
	void SetVectorParameter(const FName& ParameterName,const FVector& Value,UPCGComponent* InComp);
};
