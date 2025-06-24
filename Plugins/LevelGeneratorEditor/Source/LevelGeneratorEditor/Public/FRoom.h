#pragma once
#include "Engine/StaticMeshActor.h"

#include "FRoom.generated.h"

class ABSPRoom;

USTRUCT(BlueprintType,Blueprintable)
struct FRoom
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly,meta=(allowPrivateAccess=true),Category="RoomInformation") FVector CenterPosition;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(allowPrivateAccess=true),Category="RoomInformation") FVector Size;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(allowPrivateAccess=true),Category="RoomInformation") TArray<bool> Walls;
	UPROPERTY(VisibleAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation") bool bHorizontalSplit;
	UPROPERTY(VisibleAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation") bool bIsPathway;
	UPROPERTY(VisibleAnywhere,meta=(allowPrivateAccess=true),Category="RoomInformation") bool bSide;

	//Mesh Actors
	UPROPERTY()TArray<AStaticMeshActor*> LeftWalls;
	UPROPERTY()TArray<AStaticMeshActor*> RightWalls;
	UPROPERTY()TArray<AStaticMeshActor*> UpWalls;
	UPROPERTY()TArray<AStaticMeshActor*> DownWalls;
	UPROPERTY()TArray<AStaticMeshActor*> Floors;
	UPROPERTY()TArray<AStaticMeshActor*> Doors;
	//Aditional
	UPROPERTY() TArray<ABSPRoom*> BridgeActors;
	
	FRoom(): CenterPosition(0), Size(0), bHorizontalSplit(false), bIsPathway(false), bSide(false)
	{
		for (int32 i = 0; i < 4; i++) Walls.Add(true);
	}

	FRoom(const FVector& InPosition, const FVector& InSize,const bool& bInHorizontalSplit,const bool bInIsPathway,const bool InSide):
		CenterPosition(InPosition), Size(InSize), bHorizontalSplit(bInHorizontalSplit), bIsPathway(bInIsPathway),
		bSide(InSide)
	{
		if (!bIsPathway)
			for (int32 i = 0; i < 4; i++) Walls.Add(true);
		else
		{
			if (bInHorizontalSplit)
			{
				Walls.Add(true);
				Walls.Add(true);
				Walls.Add(false);
				Walls.Add(false);
			}
			else
			{
				Walls.Add(false);
				Walls.Add(false);
				Walls.Add(true);
				Walls.Add(true);
			}
		}
	}

	bool operator==(const FRoom& Other) const
	{
		return CenterPosition == Other.CenterPosition && Size == Other.Size && bHorizontalSplit == Other.bHorizontalSplit;
	}

};
