// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCGRoom.generated.h"


UCLASS(Blueprintable,BlueprintType)
class LEVELGENERATOREDITOR_API APCGRoomBase : public AActor
{
	GENERATED_BODY()

public:
	APCGRoomBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
UCLASS(Blueprintable,BlueprintType)
class LEVELGENERATOREDITOR_API APCGRoom : public APCGRoomBase
{
	GENERATED_BODY()
public:
	APCGRoom();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="PCG Parameters")
	void SetColumnsFloatParameter(const FName& ColumnName, const float& InValue);
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="PCG Parameters")
	void PrintTest();
	
};