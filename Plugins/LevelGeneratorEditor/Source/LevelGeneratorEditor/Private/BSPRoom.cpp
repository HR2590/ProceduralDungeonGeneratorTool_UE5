// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPRoom.h"

#include "BSPPathway.h"
#include "StaticMeshEditorSubsystem.h"
#include "StaticMeshEditorSubsystemHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"


ABSPRoom::ABSPRoom()
{
	FVector Bounds,Origin;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("Custom"));
	
	GetStaticMeshComponent()->SetCollisionObjectType(ECC_WorldStatic);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Block); 

}

void ABSPRoom::InitRoom(const FRoom& InData, const TSharedPtr<FRoomData>& InDungeonData)
{
	DungeonData = InDungeonData;
	Data = InData;
	RespawnRoom(InData);
	
	
}

void ABSPRoom::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FString PropertyChanged(PropertyChangedEvent.Property->GetName());
	const FString VectorX= GET_MEMBER_NAME_CHECKED(ABSPRoom, Data.Size.X).ToString();
	const FString VectorY= GET_MEMBER_NAME_CHECKED(ABSPRoom, Data.Size.Y).ToString();
	const FString VectorZ= GET_MEMBER_NAME_CHECKED(ABSPRoom, Data.Size.Z).ToString();
	const FString Walls= GET_MEMBER_NAME_CHECKED(ABSPRoom, Data.Walls).ToString();


	
	//-1 Not found, 10 Found
	if (VectorX.Find(PropertyChanged)>0||VectorY.Find(PropertyChanged)>0||VectorZ.Find(PropertyChanged)>0||
		Walls.Find(PropertyChanged)>0)
	{
		const auto Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(
			StaticClass(),Data.CenterPosition,FRotator::ZeroRotator);
		Room->InitRoom(Data,DungeonData);
		Destroy();
	}

	
}

void ABSPRoom::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	if (bFinished)
	{
		Data.CenterPosition=GetActorLocation();
	}

}


AStaticMeshActor* ABSPRoom::MergeStaticMeshes(UWorld* World, const TArray<AStaticMeshActor*>& StaticMeshes)
{
	if (StaticMeshes.Num() == 0 || !World) return nullptr;
	
	AStaticMeshActor* MergedMesh=NewObject<AStaticMeshActor>();
	FMergeStaticMeshActorsOptions MeshOptions;
	MeshOptions.bSpawnMergedActor=true;
	MeshOptions.MeshMergingSettings.bPivotPointAtZero=false;
	MeshOptions.bDestroySourceActors=true;
	if(!StaticMeshes[0]) return nullptr;
	MeshOptions.BasePackageName=StaticMeshes[0]->GetPackage()->GetName();
	MeshOptions.NewActorLabel=StaticMeshes[0]->GetName();
	
	UStaticMeshEditorSubsystem* MeshEditor= GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	MeshEditor->MergeStaticMeshActors(StaticMeshes,MeshOptions,MergedMesh);

	return MergedMesh;
	
	
}
void ABSPRoom::RespawnRoom(const FRoom& InRoom)
{
	FloorToMerge.Empty();
	WallsToMerge.Empty();
	FMeshDescription MergedMeshDesc;
	
	UWorld* WorldEditor = GEditor->GetEditorWorldContext().World();
	
	uint32 halfX=InRoom.Size.X/2;
	uint32 halfY=InRoom.Size.Y/2;
	
	FVector Init=FVector(
			InRoom.CenterPosition.X-halfX+*DungeonData->minMeshSizeX/2,
				InRoom.CenterPosition.Y-halfY+*DungeonData->minMeshSizeY/2,
				InRoom.CenterPosition.Z);
	
	int32 MaxX=FMath::RoundToInt32(InRoom.Size.X/(*DungeonData->minMeshSizeX));
	int32 MaxY=FMath::RoundToInt32(InRoom.Size.Y/(*DungeonData->minMeshSizeY));
	
	//UE_LOG(LogTemp,Warning,TEXT("MaxX:%d MaxY:%d"),MaxX,MaxY)
	
	for(int32 i=1;i<=MaxX;i++)
	{
		for(int32 j=1;j<=MaxY;j++)
		{
			
			AStaticMeshActor* Floor=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*DungeonData->minMeshSizeX*i,Init.Y+*DungeonData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
			Floor->GetStaticMeshComponent()->SetStaticMesh(DungeonData->MeshEntries[0]->Mesh);
			FloorToMerge.Add(Floor);

			//Left
			if(i==1&&Data.Walls[0]&&Data.Walls.Num()>0)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X-*DungeonData->minMeshSizeX/2+*DungeonData->minMeshSizeX*i,
					Init.Y+*DungeonData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(DungeonData->MeshEntries[1]->Mesh);
				WallsToMerge.Add(WallCorner);
				
			}
			//Right
			if(i>=MaxX&&Data.Walls[1]&&Data.Walls.Num()>1)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*DungeonData->minMeshSizeX/2+*DungeonData->minMeshSizeX*i,
					Init.Y+*DungeonData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(DungeonData->MeshEntries[1]->Mesh);
				WallsToMerge.Add(WallCorner);
			}
			//Down
			if(j==1&&Data.Walls[2]&&Data.Walls.Num()>2)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*DungeonData->minMeshSizeX*i,
					Init.Y-*DungeonData->minMeshSizeY/2+*DungeonData->minMeshSizeY*j,Init.Z),
				FRotator(0,90,0));
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(DungeonData->MeshEntries[1]->Mesh);
				WallsToMerge.Add(WallCorner);
			}
			//Up
			if(j>=MaxY&&Data.Walls[3]&&Data.Walls.Num()>3)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*DungeonData->minMeshSizeX*i,
					Init.Y+*DungeonData->minMeshSizeY/2+*DungeonData->minMeshSizeY*j,Init.Z),
				FRotator(0,90,0));
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(DungeonData->MeshEntries[1]->Mesh);
				WallsToMerge.Add(WallCorner);
			}
			
			
		}
	}
	
	
	if(AStaticMeshActor* FloorMesh=MergeStaticMeshes(WorldEditor, FloorToMerge))
	{
		UStaticMeshComponent* MeshComp = GetStaticMeshComponent();
		SetPivotOffset(FloorMesh->GetStaticMeshComponent()->GetStaticMesh()->GetBounds().Origin);
		SetActorLocation(InRoom.CenterPosition);
		MeshComp->SetStaticMesh(FloorMesh->GetStaticMeshComponent()->GetStaticMesh());
		
		FVector Bounds,Origin;
		GetActorBounds(false,Origin,Bounds,false);
		Bounds=FVector(Bounds.X,Bounds.Y,Bounds.Z/4);
		BoxCollision->SetWorldLocation(Origin);
		BoxCollision->SetBoxExtent(Bounds);
		BoxCollision->SetCollisionObjectType(ECC_WorldStatic);
		
		FloorMesh->Destroy();
	}

	for (const auto& Wall : WallsToMerge)
	{
		//FVector Adjust=FVector(InRoom.Size.X/4-*DungeonData->minMeshSizeX/2,InRoom.Size.Y/4-*DungeonData->minMeshSizeY/2,1);
		
		Wall->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
		//Wall->SetActorLocation(Wall->GetActorLocation()+Adjust);
	}
	
//ActorsToMerge.Empty();
}





