#pragma once
//#include "MeshEntry.h"
#include "DungeonData.generated.h"

class APCGRoom;

USTRUCT(BlueprintType,Blueprintable)
struct
FDungeonData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector initPosition;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector dimensions;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) int32 algorithmDepth;

	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector minRoomSize;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector minRoomMeshSize;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector tolerance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector minPathwaySize;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FVector minPathwayMeshSize;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<UStaticMesh*> floorMeshRoom;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<UStaticMesh*> wallMeshRoom;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<UStaticMesh*> floorMeshPathway;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<UStaticMesh*> wallMeshPathway;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<TSubclassOf<AActor>> PCGActors;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<TSubclassOf<AActor>> ChestActors;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<TSubclassOf<AActor>> DoorActors;
	
	
	FDungeonData()
	{
		initPosition = FVector::ZeroVector;

		dimensions = FVector(2000, 2000, 500);

		algorithmDepth = 4;

		minRoomSize = FVector(200,200,100);
		minRoomMeshSize = FVector(100);

		tolerance = FVector(0);
/*
		floorMeshRoom.Add(FMeshEntry(TEXT("RoomFloorMesh")));
		wallMeshRoom.Add((FMeshEntry(TEXT("RoomWallMesh"))));
		
		floorMeshPathway.Add((FMeshEntry(TEXT("PathwayFloorMesh"))));
		wallMeshPathway.Add((FMeshEntry(TEXT("PathwayWallMesh"))));*/

		minPathwaySize = minRoomSize;
		minPathwayMeshSize = FVector(100);
	}
};
