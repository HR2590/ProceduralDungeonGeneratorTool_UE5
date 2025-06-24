// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSPRoom.h"
#include "GameFramework/Actor.h"
#include "BSPDungeonGenerator.generated.h"


class UEditorUtilityWidgetBlueprint;
class SWindow;

UCLASS(Blueprintable,BlueprintType)
class LEVELGENERATOREDITOR_API UBSPDungeonGenerator : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	
	UBSPDungeonGenerator();

	void bSplit(bool& bInHorizontalSplit,const FVector& InCenter, const FVector& InSize,
				FVector& Size_Part1, FVector& Center_Part1,FVector& Size_Part2,FVector& Center_Part2);

private:
	//DungeonRoomData
	UPROPERTY()FDungeonData DungeonData;
	//Spawned
	//TArray<TArray<ABSPRoom*>> SpawnedRooms;
	UPROPERTY() TArray<ABSPRoom*> SpawnedRooms;
	UPROPERTY()TArray<ABSPRoom*> SpawnedPathways;
	UPROPERTY()TArray<AActor*> SpawnedPCG;
	UPROPERTY()TArray<AActor*> SpawnedChests;
	UPROPERTY()UEditorUtilityWidgetBlueprint* Widget;
public:	
	// Called every frame
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void BSPMeshAlgorithm(const FDungeonData& InRoomData);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void BSPRandomAlgorithm(const FDungeonData& InRoomData);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	static void DestroyRoomsFromArray(TArray<ABSPRoom*>& InArrayRooms);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteDungeon();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteRooms();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeletePathways();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteInternals();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteAllPlayerStarts();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void BuildPath(const FVector& FirstHit, const FVector& SecondHit,FVector& InCenter,FVector& InSize);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void GeneratePathways(const FDungeonData& InRoomData,bool RandomPaths);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void RandomTraceLine(ABSPRoom* InRoom,bool RandomPaths);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteNoPathwayRooms();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void MeshAlgorithmSpace(const FVector& InCenter, const FVector& InSize, const int32& InDepth,int32& InSave);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void ClearDoors(bool InVisibility,bool InClearAll);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void PackageAll();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void SpawnPCG();
	AStaticMeshActor* MergeStaticMeshes(const TArray<AStaticMeshActor*>& StaticMeshes);
	
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void LoadingBar();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DestroyLoadingBar();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	bool IsLoadingBarActive();
	
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void SelectParentActor(AActor* InActor);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void SpawnChests();

	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	TArray<ABSPRoom*> GetRooms();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void AddSpawnedPCG(AActor* InActor);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void DeleteChests();
private:
	int32 SaveDepth=-1;
	
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void PartitionSpaceRandom(const FVector& InCenter, const FVector& InSize,const int32& InDepth,int32& InSave);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void RespawnPlayerStart();
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void NormalizeVectorToSnap(FVector& InVector,bool IsPathway) const;
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	bool IsCentered(const int32& InNumber);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void SaveLoadingWidget(UEditorUtilityWidgetBlueprint* InWidget);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void UpdateChestClass(TArray<TSubclassOf<AActor>> InClass);
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true),Category="BSP Dungeon Generator")
	void UpdatePCGClass(TArray<TSubclassOf<AActor>> InClass);
};

