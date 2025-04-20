// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRoom.h"
#include "RoomData.h"
#include "Engine/StaticMeshActor.h"
#include "BSPRoom.generated.h"

class UBoxComponent;

UCLASS()
class LEVELGENERATOREDITOR_API ABSPRoom : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABSPRoom();
	// Sets default values for this actor's properties
	void InitRoom(const FRoom& InData, const TSharedPtr<FRoomData>& InDungeonData);
	UPROPERTY(EditAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation")FRoom Data;

protected:
	// Called when the game starts or when spawned
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision") UBoxComponent* BoxCollision;
	UPROPERTY()TArray<AStaticMeshActor*> FloorToMerge;
	UPROPERTY()TArray<AStaticMeshActor*> WallsToMerge;
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;

	static AStaticMeshActor* MergeStaticMeshes(UWorld* World, const TArray<AStaticMeshActor*>& StaticMeshes);
	void RespawnRoom(const FRoom& InRoom);
	

private:
	TSharedPtr<FRoomData> DungeonData;

};
