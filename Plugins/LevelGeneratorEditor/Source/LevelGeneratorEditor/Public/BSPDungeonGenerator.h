// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSPRoom.h"
#include "FRoom.h"
#include "RoomData.h"
#include "GameFramework/Actor.h"
#include "BSPDungeonGenerator.generated.h"


UCLASS()
class LEVELGENERATOREDITOR_API UBSPDungeonGenerator : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	
	UBSPDungeonGenerator();

	void bSplit(const bool& InHorizontalSplit, const FVector& InCenter, const FVector& InSize,
				FVector& Size_Part1, FVector& Center_Part1,FVector& Size_Part2,FVector& Center_Part2);

protected:
	//DungeonRoomData
	TSharedPtr<FRoomData> RoomData;
	TMap<uint32,TArray<TArray<FRoom>>> Rooms;
	UPROPERTY()TArray<FRoom> Pathways;
	//Spawned
	TArray<TArray<ABSPRoom*>> SpawnedRooms;
	UPROPERTY()TArray<ABSPRoom*> SpawnedPathways;
	
public:
	// Called every frame
	void BSPUniformAlgorithm();
	void BSPRandomAlgorithm();
	void AssignData(const TSharedPtr<FRoomData>& InRoomData);
	void DeleteAll();
	void BuildPath(const FRoom& FirstRoom, const FRoom& SecondRoom,FVector& InCenter,FVector& InSize);
	void CreatePairPathWaysAndClean();

	void UnifyPairRooms();
	void RandomTraceLine(const TArray<ABSPRoom*>& InPair);
private:
	uint32 SaveDepth=-1;
	
	bool PartitionSpace(const FVector& InCenter, const FVector& InSize,const uint32& InDepth);
	void PartitionSpaceRandom(const FVector& InCenter, const FVector& InSize,const uint32& InDepth,uint32& InSave);

};

