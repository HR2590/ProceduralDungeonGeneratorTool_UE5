// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRoom.h"
#include "DungeonData.h"
#include "Engine/StaticMeshActor.h"
#include "BSPRoom.generated.h"

class UBoxComponent;

UCLASS()
class LEVELGENERATOREDITOR_API ABSPRoom : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABSPRoom();

	void InitRoom(const FRoom& InData, const FDungeonData& InDungeonData);
	UPROPERTY(EditAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation")
	FRoom Data;
	UFUNCTION(BlueprintCallable,BlueprintPure,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	FRoom GetRoomData();
	void CleanMeshes();
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
protected:
	
	virtual void Destroyed() override;
	
	void CreateRoom(const FRoom& InRoom);
	AStaticMeshActor* SpawnWall(const FVector& InLocation,const FRotator& InRotator, bool bIsPathway);
	void SpawnFloor(const FVector& InLocation,bool bIsPathway);
	bool CheckWallDoor(const FVector& InDoor);

private:
	FDungeonData DungeonData;
	
};
