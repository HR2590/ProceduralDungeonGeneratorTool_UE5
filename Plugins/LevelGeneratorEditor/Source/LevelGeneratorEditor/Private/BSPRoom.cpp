// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPRoom.h"
#include "Engine/StaticMeshActor.h"


ABSPRoom::ABSPRoom()
{

}


void ABSPRoom::InitRoom(const FRoom& InData, const FDungeonData& InDungeonData)
{
	DungeonData = InDungeonData;
	Data = InData;
	
	SetActorLocation(InData.CenterPosition);
	CreateRoom(InData);
	
	
}


FRoom ABSPRoom::GetRoomData()
{
	return Data;
}

void ABSPRoom::CleanMeshes()
{
	if(!Data.Floors.IsEmpty())
		for (auto& Floor : Data.Floors)if(Floor)Floor->Destroy();Data.Floors.Empty();
	if(!Data.LeftWalls.IsEmpty())
		for (auto& Wall : Data.LeftWalls)if(Wall)Wall->Destroy();Data.LeftWalls.Empty();
	if(!Data.RightWalls.IsEmpty())
		for (auto& Wall : Data.RightWalls)if(Wall)Wall->Destroy();Data.RightWalls.Empty();
	if(!Data.UpWalls.IsEmpty())
		for (auto& Wall : Data.UpWalls)  if(Wall)Wall->Destroy();Data.UpWalls.Empty();
	if(!Data.DownWalls.IsEmpty())
		for (auto& Wall : Data.DownWalls)  if(Wall)Wall->Destroy();Data.DownWalls.Empty();
	if(!Data.Doors.IsEmpty())
		for (auto& Wall : Data.Doors)  if(Wall)Wall->Destroy();Data.Doors.Empty();
	
}

void ABSPRoom::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	FString PropertyChanged= PropertyChangedEvent.GetMemberPropertyName().ToString();
	FString DataProperty=GET_MEMBER_NAME_CHECKED(ThisClass,Data).ToString();
	if(PropertyChanged.Find(DataProperty)>=0)
	{
		InitRoom(Data,DungeonData);
	}
	
}

void ABSPRoom::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

		if(bFinished)
		{
			Data.CenterPosition = GetActorLocation();
		}
}


void ABSPRoom::CreateRoom(const FRoom& InRoom)
{
	Destroyed();
	FMeshDescription MergedMeshDesc;
	
	FVector MeshSize,MinSize;
	int32 ZAdjust;
	float WallAdjust=90;
	if(InRoom.bIsPathway)
	{
		MeshSize=FVector(DungeonData.minPathwayMeshSize);
		MinSize=FVector(DungeonData.minPathwaySize);
		ZAdjust=DungeonData.floorMeshPathway[0]->GetBounds().BoxExtent.Z*2;
	}
	else
	{
		MeshSize=FVector(DungeonData.minRoomMeshSize);
		MinSize=FVector(DungeonData.minRoomSize);
		ZAdjust=DungeonData.floorMeshRoom[0]->GetBounds().BoxExtent.Z*2;
	}
		
	
	const uint32 halfX=InRoom.Size.X/2;
	const uint32 halfY=InRoom.Size.Y/2;
	const uint32 halfZ=InRoom.Size.Z/2;

	
	const FVector Init=FVector(
			InRoom.CenterPosition.X-halfX+MeshSize.X/2,
				InRoom.CenterPosition.Y-halfY+MeshSize.Y/2,
				InRoom.CenterPosition.Z);

	const int32 MaxX=FMath::RoundToInt32(InRoom.Size.X/(MeshSize.X));
	const int32 MaxY=FMath::RoundToInt32(InRoom.Size.Y/(MeshSize.Y));
	const int32 MaxZ=FMath::RoundToInt32(MinSize.Z/MeshSize.Z);
	
	const int32 HalfMaxX=FMath::RoundToInt32(MaxX/2.0f);
	const int32 HalfMaxY=FMath::RoundToInt32(MaxY/2.0f);
	const int32 HalfMaxZ=FMath::RoundToInt32(MaxZ/2.0f);
	
	for(int32 i=1;i<=MaxX;i++)
	{
		for(int32 j=1;j<=MaxY;j++)
		{
			for(int32 k=0;k<MaxZ;k++)
			{
				//Floor
				SpawnFloor(FVector(Init.X+MeshSize.X*i,Init.Y+MeshSize.Y*j,Init.Z-ZAdjust),InRoom.bIsPathway);

				//WallLeft
				if(i==1&&Data.Walls.Num()>0)
				{
					
					FVector WallPosition=FVector(Init.X-MeshSize.X/2+MeshSize.X*i,Init.Y+MeshSize.Y*j,
						Init.Z+MeshSize.Z*k);
				
					if(!(HalfMaxY==j&&!Data.Walls[0]))
					{
						if(CheckWallDoor(WallPosition)) continue;
						//Data.LeftWalls.Add(SpawnWall(WallPosition,FRotator::ZeroRotator,InRoom.bIsPathway));
						Data.LeftWalls.Add(SpawnWall(WallPosition,FRotator(0,-WallAdjust,0),InRoom.bIsPathway));
					}
				
				
				}
				//WallRight
				if(i>=MaxX&&Data.Walls.Num()>1)
				{
					FVector WallPosition=FVector(Init.X+MeshSize.X/2+MeshSize.X*i,
						Init.Y+MeshSize.Y*j,Init.Z+MeshSize.Z*k);

					if(!(HalfMaxY==j&&!Data.Walls[1]))
					{
						if(CheckWallDoor(WallPosition)) continue;
						//Data.RightWalls.Add(SpawnWall(WallPosition,FRotator::ZeroRotator,InRoom.bIsPathway));
						Data.RightWalls.Add(SpawnWall(WallPosition,FRotator(0,WallAdjust,0),InRoom.bIsPathway));
					}
				
	
				}
			//WallDown
			if(j==1&&Data.Walls.Num()>2)
			{
				FVector WallPosition=FVector(Init.X+MeshSize.X*i,
					Init.Y-MeshSize.Y/2+MeshSize.Y*j,Init.Z+MeshSize.Z*k);

				if(!(HalfMaxX==i&&!Data.Walls[2]))
				{
					if(CheckWallDoor(WallPosition)) continue;
					Data.DownWalls.Add(SpawnWall(WallPosition,FRotator(0,0,0),InRoom.bIsPathway));
				}
				
			}
			//WallUp
			if(j>=MaxY&&Data.Walls.Num()>3)
			{
				FVector WallPosition=FVector(Init.X+MeshSize.X*i,Init.Y+MeshSize.Y/2+MeshSize.Y*j,
					Init.Z+MeshSize.Z*k);
				
				if(!(HalfMaxX==i&&!Data.Walls[3]))
				{
					if(CheckWallDoor(WallPosition)) continue;
					Data.UpWalls.Add(SpawnWall(WallPosition,FRotator(0,WallAdjust*2,0),InRoom.bIsPathway));
				}
				
				}
			}
			
		}
	}
	
	
}

AStaticMeshActor* ABSPRoom::SpawnWall(const FVector& InLocation,const FRotator& InRotator, bool bIsPathway)
{
	UStaticMesh* Mesh;
	FVector MeshSize;
	uint32 RandomIndex;
	UWorld* WorldEditor = GEditor->GetEditorWorldContext().World();
	
	const auto Wall=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
	                                                          InLocation,InRotator);
	
	bIsPathway?RandomIndex=FMath::RandRange(0,DungeonData.wallMeshPathway.Num()-1)
	           :RandomIndex=FMath::RandRange(0,DungeonData.wallMeshRoom.Num()-1);
	bIsPathway?Mesh=DungeonData.wallMeshPathway[RandomIndex]:Mesh=DungeonData.wallMeshRoom[RandomIndex];
	bIsPathway?MeshSize=DungeonData.minPathwayMeshSize:MeshSize=DungeonData.minRoomMeshSize;
	
	Wall->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	
	Wall->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	Wall->SetActorLocation(Wall->GetActorLocation()-FVector(MeshSize.X,MeshSize.Y,0));
	
	return Wall;
}

void ABSPRoom::SpawnFloor(const FVector& InLocation,bool bIsPathway)
{
	UStaticMesh* Mesh;
	FVector MeshSize;
	uint32 RandomIndex;
	UWorld* WorldEditor = GEditor->GetEditorWorldContext().World();

	const auto Floor=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
	                                                           InLocation,FRotator::ZeroRotator);

	bIsPathway?RandomIndex=FMath::RandRange(0,DungeonData.floorMeshPathway.Num()-1)
			   :RandomIndex=FMath::RandRange(0,DungeonData.floorMeshRoom.Num()-1);
	bIsPathway?Mesh=DungeonData.floorMeshPathway[RandomIndex]:Mesh=DungeonData.floorMeshRoom[RandomIndex];
	bIsPathway?MeshSize=DungeonData.minPathwayMeshSize:MeshSize=DungeonData.minRoomMeshSize;
	
	Floor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	Data.Floors.Add(Floor);

	Floor->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	Floor->SetActorLocation(Floor->GetActorLocation()-FVector(MeshSize.X,MeshSize.Y,0));
	
}

bool ABSPRoom::CheckWallDoor(const FVector& InDoor)
{
	//UE_LOG(LogTemp,Warning,TEXT("LastDiffSize:%s"),*(LastDiffSize).ToString());
	//UE_LOG(LogTemp,Warning,TEXT("LastDiffPos:%s"),*(LastDiffPosition).ToString());
	//UE_LOG(LogTemp,Warning,TEXT("CreateDoor:%s"),*(InDoor).ToString());
	/*
	for (auto& Door:Data.Doors)
	{
		//UE_LOG(LogTemp,Warning,TEXT("SavedValue:%s"),*(Door.Key).ToString());
		if(InDoor==Door.Key)
		{
			UE_LOG(LogTemp,Warning,TEXT("Found"));
			return true;
		}
	
	}
*/
	return false;
}




void ABSPRoom::Destroyed()
{
	Super::Destroyed();
	CleanMeshes();
}






