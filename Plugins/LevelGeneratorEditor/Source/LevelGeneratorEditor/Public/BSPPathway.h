// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSPDungeonGenerator.h"
#include "GameFramework/Actor.h"
#include "BSPPathway.generated.h"

UCLASS()
class LEVELGENERATOREDITOR_API ABSPPathway : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABSPPathway();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation")
	FRoom Data;


};
