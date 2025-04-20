// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPDungeonGenerator.h"
#include "BSPRoom.h"


#define MinToleranceX 0.0f
#define MinToleranceY 0.0f


UBSPDungeonGenerator::UBSPDungeonGenerator()
{
	
}

// Called when the game starts or when spawned


void UBSPDungeonGenerator::bSplit(const bool& InHorizontalSplit, const FVector& InCenter, const FVector& InSize,
								FVector& Size_Part1, FVector& Center_Part1,FVector& Size_Part2,FVector& Center_Part2)
{
	uint32 Difference,TempDivider;
	
	if (!InHorizontalSplit)
	{
		
		Difference=Size_Part1.X/2;
		TempDivider=InSize.X/2;
		
		Size_Part1=FVector(TempDivider-*RoomData->minToleranceX,InSize.Y,Size_Part1.Z);
		Size_Part2=FVector(TempDivider-*RoomData->minToleranceX,InSize.Y,Size_Part2.Z);

		Center_Part1=FVector(InCenter.X+Difference,InCenter.Y,InCenter.Z);
		Center_Part2=FVector(InCenter.X-Difference,InCenter.Y,InCenter.Z);

	}
	else
	{
		Difference=Size_Part1.Y/2;
		TempDivider=InSize.Y/2;
		
		Size_Part1=FVector(InSize.X,TempDivider-*RoomData->minToleranceY,Size_Part1.Z);
		Size_Part2=FVector(InSize.X,TempDivider-*RoomData->minToleranceY,Size_Part2.Z);

		Center_Part1=FVector(InCenter.X,InCenter.Y+Difference,InCenter.Z);
		Center_Part2=FVector(InCenter.X,InCenter.Y-Difference,InCenter.Z);
	}

}

bool UBSPDungeonGenerator::PartitionSpace(const FVector& InCenter, const FVector& InSize, const uint32& InDepth)
{

	/*
	const int32 ActualDepth=InDepth;
	
	bool State2;
	bool State1 = State2 = false;
	bool bSplitState=FMath::RandBool();
	
	FVector Size_Part1 = FVector(InSize.X/2,InSize.Y/2,InSize.Z );
	FVector Size_Part2 = Size_Part1;
	
	FVector Center_Part1 = InCenter;
	FVector Center_Part2 = Center_Part1;

	uint32 SizeX=static_cast<uint32>(InSize.X);
	uint32 SizeY=static_cast<uint32>(InSize.Y);
	
	if (SizeX<SizeY&&
		SizeX>=(*RoomData->minRoomSizeX+*RoomData->minToleranceX)&&
		SizeY>=(*RoomData->minRoomSizeY+*RoomData->minToleranceY)&&
		!FMath::IsNegativeOrNegativeZero(InSize.X)&&!FMath::IsNegativeOrNegativeZero(InSize.Y))
	{
			bSplitState=true;
			bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
	}
	else if(SizeY<SizeX&&SizeX>=*RoomData->minRoomSizeX&&SizeY>=*RoomData->minRoomSizeY)
	{
			bSplitState=false;
			bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
	}
	else if(SizeX>=*RoomData->minRoomSizeX&&SizeY>=*RoomData->minRoomSizeY)
	{
		
		if(SizeX>=*RoomData->minRoomSizeX*2&&SizeY>=*RoomData->minRoomSizeY*2)
			bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
		else if(SizeX>=*RoomData->minRoomSizeX*2)
		{
			bSplitState=false;
			bSplit(bSplitState,InCenter, InSize,Size_Part1, Center_Part1, Size_Part2, Center_Part2);
		}
		else if(SizeY>=*RoomData->minRoomSizeY*2)
		{
			bSplitState=true;
			bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
		}
		else
			bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
	}
	else
	{
		SaveDepth=ActualDepth-1;
		return false;
	}
	State1=PartitionSpace(Center_Part1, Size_Part1, ActualDepth+1);
	State2=PartitionSpace(Center_Part2, Size_Part2, ActualDepth+1);

	if(!State1&&!State2)
	{
		if (Rooms.Find(ActualDepth))
		{
			Rooms[ActualDepth].Add(FRoom(InCenter, InSize,bSplitState));
		}
		else
		{
			TArray<FRoom> TempRooms;
			TempRooms.Add(FRoom(InCenter, InSize,bSplitState));
			Rooms.Add(ActualDepth,TempRooms);
		}
		
	}

return true;
*/
	return true;
}

void UBSPDungeonGenerator::PartitionSpaceRandom(const FVector& InCenter, const FVector& InSize,const uint32& InDepth,uint32& InSave)
{
	TArray<FRoom> ActualRooms;
	
	const uint32 ActualDepth=InDepth;
	
	uint32 SaveLeft=0;
	uint32 SaveRight=SaveLeft;

	bool bSplitState=false;
	
	FVector Size_Part1 = FVector(InSize.X/2,InSize.Y/2,InSize.Z );
	FVector Size_Part2 = Size_Part1;
	
	FVector Center_Part1 = InCenter;
	FVector Center_Part2 = Center_Part1;
	
	uint32 SizeX=static_cast<uint32>(InSize.X);
	uint32 SizeY=static_cast<uint32>(InSize.Y);
	
	if (ActualDepth<=*RoomData->Depth&&
		SizeX>=*RoomData->minRoomSizeX+*RoomData->minToleranceX&&
		SizeY>=*RoomData->minRoomSizeX+*RoomData->minToleranceY&&
		!FMath::IsNegativeOrNegativeZero(InSize.X)&&!FMath::IsNegativeOrNegativeZero(InSize.Y))
	{
		uint32 minRoomDivider;

		if (SizeX>SizeY)
		{
			minRoomDivider=*RoomData->minRoomSizeX+*RoomData->minToleranceX+MinToleranceX;
			float DividerX = FMath::RoundToInt32(FMath::RandRange(minRoomDivider, InSize.X-minRoomDivider)/(*RoomData->minRoomSizeX))**RoomData->minRoomSizeX;
			float SizeUnit2=FMath::RoundToInt32(InSize.X-DividerX);

			Size_Part1=FVector(DividerX-MinToleranceX,InSize.Y,InSize.Z);
			Size_Part2=FVector(SizeUnit2-MinToleranceX,InSize.Y,InSize.Z);
			
			Center_Part1=FVector(InCenter.X-(SizeUnit2/(*RoomData->DivisorX)),InCenter.Y,InCenter.Z);
			Center_Part2=FVector(InCenter.X+(DividerX),InCenter.Y,InCenter.Z);
		
		}
		else
		{
			bSplitState=true;
			minRoomDivider=*RoomData->minRoomSizeY+*RoomData->minToleranceY+MinToleranceY;
			float DividerY = FMath::RoundToInt32(FMath::RandRange(minRoomDivider, InSize.Y-minRoomDivider)/(*RoomData->minRoomSizeY))**RoomData->minRoomSizeY;
			float SizeUnit2=FMath::RoundToInt32(InSize.Y-DividerY);
			
			Size_Part1=FVector(InSize.X,DividerY-MinToleranceY,InSize.Z);
			Size_Part2=FVector(InSize.X,SizeUnit2-MinToleranceY,InSize.Z);
			
			Center_Part1=FVector(InCenter.X,InCenter.Y-(SizeUnit2/(*RoomData->DivisorY)),InCenter.Z);
			Center_Part2=FVector(InCenter.X,InCenter.Y+(DividerY),InCenter.Z);
		}

	}
	else
	{
		InSave=3;
		return;
	}
	
	PartitionSpaceRandom(Center_Part1, Size_Part1, ActualDepth+1,SaveLeft);
	PartitionSpaceRandom(Center_Part2, Size_Part2, ActualDepth+1,SaveRight);
	if(SaveLeft==3&&SaveRight==3) InSave=1;
	if(SaveLeft==1) ActualRooms.Add(FRoom(Center_Part1, Size_Part1,bSplitState,false));
	if(SaveRight==1) ActualRooms.Add(FRoom(Center_Part2, Size_Part2,bSplitState,false));

	if(ActualRooms.Num()!=0)
	{
		Rooms.FindOrAdd(ActualDepth);
		Rooms.Find(ActualDepth)->Add(ActualRooms);
	}
	
}



/*
void UBSPDungeonGenerator::SpawnRoom(const FRoom& InRoom)
{
	TArray<AStaticMeshActor*> ActorsToMerge;

	FMeshDescription MergedMeshDesc;
	FStaticMeshAttributes MergedAttributes(MergedMeshDesc);
	MergedAttributes.Register();
	
	UWorld* WorldEditor = GEditor->GetEditorWorldContext().World();


	uint32 halfX=InRoom.Size.X/2;
	uint32 halfY=InRoom.Size.Y/2;
	
	
	FVector Init=FVector(
			InRoom.CenterPosition.X-halfX+*RoomData->minMeshSizeX/2,
				InRoom.CenterPosition.Y-halfY+*RoomData->minMeshSizeY/2,
				InRoom.CenterPosition.Z);
	
	int32 MaxX=FMath::RoundToInt32(InRoom.Size.X/(*RoomData->minMeshSizeX));
	int32 MaxY=FMath::RoundToInt32(InRoom.Size.Y/(*RoomData->minMeshSizeY));
	
	for(int i=1;i<=MaxX;i++)
	{
		for(int j=1;j<=MaxY;j++)
		{
			
			AStaticMeshActor* Floor=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*RoomData->minMeshSizeX*i,Init.Y+*RoomData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
			Floor->GetStaticMeshComponent()->SetStaticMesh(RoomData->MeshEntries[0]->Mesh);
			ActorsToMerge.Add(Floor);
			
			if(i==1)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X-*RoomData->minMeshSizeX/2+*RoomData->minMeshSizeX*i,
					Init.Y+*RoomData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(RoomData->MeshEntries[1]->Mesh);
				ActorsToMerge.Add(WallCorner);
				
			}
			
			if(i>=MaxX)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*RoomData->minMeshSizeX/2+*RoomData->minMeshSizeX*i,
					Init.Y+*RoomData->minMeshSizeY*j,Init.Z),
				FRotator::ZeroRotator);
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(RoomData->MeshEntries[1]->Mesh);
				ActorsToMerge.Add(WallCorner);
			}
			
			if(j==1)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*RoomData->minMeshSizeX*i,
					Init.Y-*RoomData->minMeshSizeY/2+*RoomData->minMeshSizeY*j,Init.Z),
				FRotator(0,90,0));
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(RoomData->MeshEntries[1]->Mesh);
				ActorsToMerge.Add(WallCorner);
			}
			
			if(j>=MaxY)
			{
				AStaticMeshActor* WallCorner=WorldEditor->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
				FVector(Init.X+*RoomData->minMeshSizeX*i,
					Init.Y+*RoomData->minMeshSizeY/2+*RoomData->minMeshSizeY*j,Init.Z),
				FRotator(0,90,0));
				WallCorner->GetStaticMeshComponent()->SetStaticMesh(RoomData->MeshEntries[1]->Mesh);
				ActorsToMerge.Add(WallCorner);
			}
			
			
			
			
		}
	}
	AStaticMeshActor* NewFloorMesh= MergeStaticMeshes(WorldEditor, ActorsToMerge);
	if(NewFloorMesh)
	{
		NewFloorMesh->SetPivotOffset(NewFloorMesh->GetStaticMeshComponent()->GetStaticMesh()->GetBounds().Origin);
		NewFloorMesh->SetActorLocation(FVector(InRoom.CenterPosition));
	}
	SpawnedActors.Add(NewFloorMesh);
	ActorsToMerge.Empty();
	
	/*AStaticMeshActor* NewWallMesh = MergeStaticMeshes(WorldEditor, WallsToMerge);
	if(ensure(NewWallMesh))
	{
		NewWallMesh->SetPivotOffset(NewWallMesh->GetStaticMeshComponent()->GetStaticMesh()->GetBounds().Origin);
		NewWallMesh->SetActorLocation(FVector(InRoom.CenterPosition));
	}
	
	
}


AStaticMeshActor* UBSPDungeonGenerator::MergeStaticMeshes(UWorld* World, const TArray<AStaticMeshActor*>& StaticMeshes)
{
	if (StaticMeshes.Num() == 0 || !World) return nullptr;
	
	AStaticMeshActor* MergedMesh=NewObject<AStaticMeshActor>(World);
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
	
	
}*/

void UBSPDungeonGenerator::BSPUniformAlgorithm()
{
	DeleteAll();
	
	if(!RoomData->MeshEntries[0]->Mesh||!RoomData->MeshEntries[1]->Mesh) return;

	PartitionSpace(FVector(*RoomData->InitPosition), FVector(*RoomData->width, *RoomData->height,1), 0);
	CreatePairPathWaysAndClean();
	
	for (const FRoom& InPathway : Pathways)
	{
		ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
			InPathway.CenterPosition,FRotator::ZeroRotator);
		Room->InitRoom(InPathway,RoomData);
	}
	
	for (const auto& InDepth : Rooms)
		for (auto& InArray : InDepth.Value)
			for (auto& InRoom: InArray)
				{
				ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(
					ABSPRoom::StaticClass(),InRoom.CenterPosition,FRotator::ZeroRotator);
				Room->InitRoom(InRoom,RoomData);
				}
	
	
}

void UBSPDungeonGenerator::BSPRandomAlgorithm()
{
	uint32 Save=0;
	DeleteAll();

	if(!RoomData->MeshEntries[0]->Mesh||!RoomData->MeshEntries[1]->Mesh) return;
	

	
	PartitionSpaceRandom(FVector(*RoomData->InitPosition), FVector(*RoomData->width, *RoomData->height,1), 0,Save);
	CreatePairPathWaysAndClean();
	
	FActorSpawnParameters ObjectParams;
	ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
	ObjectParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		for (const FRoom& InPathway : Pathways)
		{
			ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
				InPathway.CenterPosition,FRotator::ZeroRotator,ObjectParams);
			Room->InitRoom(InPathway,RoomData);
			SpawnedPathways.Add(Room);
			
		}
	
	for (const auto& InDepth : Rooms)
	{
		for (auto& InArray : InDepth.Value)
		{
			TArray<ABSPRoom*> PairRooms;
			for (auto& InRoom: InArray)
			{
				ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
				InRoom.CenterPosition,FRotator::ZeroRotator,ObjectParams);
				Room->InitRoom(InRoom,RoomData);
				PairRooms.Add(Room);
			}
			SpawnedRooms.Add(PairRooms);
		}
	}
	
	UnifyPairRooms();


	
}

void UBSPDungeonGenerator::AssignData(const TSharedPtr<FRoomData>& InRoomData)
{
	RoomData = InRoomData;
}

void UBSPDungeonGenerator::DeleteAll()
{
	
	Pathways.Empty();
	Rooms.Empty();
	
	
	for (auto& Pairs : SpawnedRooms)
		for (auto& ActorRoom : Pairs)
			if (ActorRoom)ActorRoom->Destroy();
		

	for (auto& Pathway : SpawnedPathways)
		if (Pathway)Pathway->Destroy();
	
	SpawnedPathways.Empty();
	SpawnedRooms.Empty();
}

void UBSPDungeonGenerator::BuildPath(const FRoom& FirstRoom, const FRoom& SecondRoom,FVector& InCenter,FVector& InSize)
{
	float difx=FirstRoom.CenterPosition.X-SecondRoom.CenterPosition.X;
	float dify=FirstRoom.CenterPosition.Y-SecondRoom.CenterPosition.Y;

	
	
	if (FMath::Abs(difx)<FMath::Abs(dify))
	{
		float UpperLimit;
		float LowerLimit;

		if(FirstRoom.CenterPosition.Y<SecondRoom.CenterPosition.Y)
		{
			UpperLimit=SecondRoom.CenterPosition.Y;
			LowerLimit=FirstRoom.CenterPosition.Y+FirstRoom.Size.Y;
			if(UpperLimit<LowerLimit)
			{
				const float Temp=UpperLimit;
				UpperLimit=LowerLimit;
				LowerLimit=Temp;
			}
			
			InSize=FVector(*RoomData->minMeshSizeX,UpperLimit-LowerLimit,1);

			InCenter=FVector(
				FirstRoom.CenterPosition.X+FirstRoom.Size.X/2,
				FMath::RoundToInt32((UpperLimit+LowerLimit)/2-InSize.Y/2),
				FirstRoom.CenterPosition.Z);
		}
		else
		{
			
			UpperLimit=FirstRoom.CenterPosition.Y;
			LowerLimit=SecondRoom.CenterPosition.Y+SecondRoom.Size.Y;

			if(UpperLimit<LowerLimit)
			{
				const float Temp=UpperLimit;
				UpperLimit=LowerLimit;
				LowerLimit=Temp;
			}
			
			InSize=FVector(*RoomData->minMeshSizeX,UpperLimit-LowerLimit,1);

			InCenter=FVector(
				FirstRoom.CenterPosition.X+FirstRoom.Size.X/2,
				FMath::RoundToInt32((UpperLimit+LowerLimit)/2-InSize.Y/2),
				SecondRoom.CenterPosition.Z);
		}
		
	}
	else
	{
		float RightLimit;
		float LeftLimit;
		
		if(FirstRoom.CenterPosition.X<SecondRoom.CenterPosition.X)
		{
			RightLimit=SecondRoom.CenterPosition.X;
			LeftLimit=FirstRoom.CenterPosition.X+FirstRoom.Size.X;
			if(RightLimit<LeftLimit)
			{
				const float Temp=RightLimit;
				RightLimit=LeftLimit;
				LeftLimit=Temp;
			}
			InSize=FVector(RightLimit-LeftLimit,*RoomData->minMeshSizeY,1);
			InCenter=FVector(
				FMath::RoundToInt32((RightLimit+LeftLimit)/2-InSize.X/2),
				FirstRoom.CenterPosition.Y+FirstRoom.Size.Y/2,FirstRoom.CenterPosition.Z);
			
		}
		
		else
		{
			RightLimit=FirstRoom.CenterPosition.X;
			LeftLimit=SecondRoom.CenterPosition.X+SecondRoom.Size.X;
			if(RightLimit<LeftLimit)
			{
				const float Temp=RightLimit;
				RightLimit=LeftLimit;
				LeftLimit=Temp;
			}
			
			InSize=FVector(RightLimit-LeftLimit,*RoomData->minMeshSizeY,1);
			InCenter=FVector(
				FMath::RoundToInt32((RightLimit+LeftLimit)/2-InSize.X/2),
				FirstRoom.CenterPosition.Y+FirstRoom.Size.Y/2,SecondRoom.CenterPosition.Z);
		}
			
		
		

						
	}

	
}

void UBSPDungeonGenerator::CreatePairPathWaysAndClean()
{
	for (auto& InDepth : Rooms)
	{
		for (int i=0; i<InDepth.Value.Num();i++)
		{
			if(InDepth.Value[i].Num()==2)
			{
				FRoom FirstRoom = InDepth.Value[i][0];
				FRoom LastRoom = InDepth.Value[i][1];
				FVector PathCenter,PathSize;
				BuildPath(FirstRoom, LastRoom,PathCenter,PathSize);
				Pathways.AddUnique(
					FRoom(PathCenter,PathSize,FirstRoom.bHorizontalSplit,true));
				
			}
			else InDepth.Value[i].Empty();
		
			
		}
	}
	
}



void UBSPDungeonGenerator::UnifyPairRooms()
{
	for (auto& PairRooms : SpawnedRooms)
		if(PairRooms.Num()==2) RandomTraceLine(PairRooms);
	
}




void UBSPDungeonGenerator::RandomTraceLine(const TArray<ABSPRoom*>& InPair)
{
	float LimitX=RoomData->InitPosition->X+*RoomData->width;
	float LimitY=RoomData->InitPosition->Y+*RoomData->height;

	
	UWorld* World=GEditor->GetEditorWorldContext().World();
	bool bHorizontalSplit=false;
	
	FHitResult Hit,Hit1,Hit2;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams QueryParams;
	FActorSpawnParameters ActorParams;
	
	ActorParams.ObjectFlags=RF_Transactional|RF_Public;
	ActorParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	for (auto& InRoom : InPair)
	for(SIZE_T i=1;i<=4;i++)
		{
			
			QueryParams.AddIgnoredActor(InRoom);
		
			FVector Start=InRoom->Data.CenterPosition+
				FVector(InRoom->Data.Size.X/2-*RoomData->minMeshSizeX/2,
					InRoom->Data.Size.Y/2-*RoomData->minMeshSizeY/2,*RoomData->minMeshSizeY/2);
			
			FVector End,PosAdjust,NegAdjust;
		
			Hit.Reset();Hit1.Reset();Hit2.Reset();
	
			switch (i)
			{
			case 1:
				PosAdjust=FVector(0,*RoomData->minMeshSizeY,0);
				End=FVector(FMath::Clamp(Start.X+*RoomData->width,LimitX*-1,LimitX),Start.Y,Start.Z);
				bHorizontalSplit=false;
				break;
			case 2:
				PosAdjust=FVector(0,*RoomData->minMeshSizeY,0);
				End=FVector(FMath::Clamp(Start.X-*RoomData->width,LimitX*-1,LimitX),Start.Y,Start.Z);
				bHorizontalSplit=false;
				break;
			case 3:
				PosAdjust=FVector(*RoomData->minMeshSizeX,0,0);
				End=FVector(Start.X,FMath::Clamp(Start.Y+*RoomData->height,LimitY*-1,LimitY),Start.Z);
				bHorizontalSplit=true;
				break;
			case 4:
				PosAdjust=FVector(*RoomData->minMeshSizeX,0,0);
				End=FVector(Start.X,FMath::Clamp(Start.Y-*RoomData->height,LimitY*-1,LimitY),Start.Z);
				bHorizontalSplit=true;
				break;
			default:
				break;
			}

			NegAdjust=PosAdjust*-1;
		
			World->LineTraceSingleByObjectType(Hit1,Start+PosAdjust,End+PosAdjust,ObjectParams,QueryParams);
			World->LineTraceSingleByObjectType(Hit,Start,End,ObjectParams,QueryParams);
			World->LineTraceSingleByObjectType(Hit2,Start+NegAdjust,End+NegAdjust,ObjectParams,QueryParams);
		
			//DrawDebugLine(World, Start+Adjust, End+Adjust, FColor::Green,false,60.0f, 0, 2.0f);
			//DrawDebugLine(World, Start, End, FColor::Green,false,60.0f, 0, 2.0f);
			//DrawDebugLine(World, Start-Adjust, End-Adjust, FColor::Green,false,60.0f, 0, 2.0f);
			if (Hit.bBlockingHit&&Hit1.bBlockingHit&&Hit2.bBlockingHit)
			{
				if(Hit.GetActor()==Hit1.GetActor()&&Hit1.GetActor()==Hit2.GetActor())
				{
					//DrawDebugPoint(World,Hit.ImpactPoint,5.0f,FColor::Red,true,-1,0);
					//DrawDebugPoint(World,Hit1.ImpactPoint,5.0f,FColor::Red,true,-1,0);
					//DrawDebugPoint(World,Hit2.ImpactPoint,5.0f,FColor::Red,true,-1,0);
					ABSPRoom* HitActor=Cast<ABSPRoom>(Hit.GetActor());
					if (HitActor&&HitActor!=InPair[0]&&HitActor!=InPair[1]&&!HitActor->Data.bIsPathway)
					{
						FVector PathCenter,PathSize;
						BuildPath(InRoom->Data, HitActor->Data,PathCenter,PathSize);
			
						ABSPRoom* Room=World->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
							PathCenter,FRotator::ZeroRotator,ActorParams);
						
						Room->InitRoom(FRoom(PathCenter,PathSize,bHorizontalSplit,true),RoomData);
						SpawnedPathways.Add(Room);
					}
				}
			}

		else if (Hit1.bBlockingHit)
		{
			if(Hit1.GetActor())
			{
				ABSPRoom* HitActor=Cast<ABSPRoom>(Hit1.GetActor());
				if(HitActor&&HitActor!=InPair[0]&&HitActor!=InPair[1]&&!HitActor->Data.bIsPathway)
				{

					FVector PathCenter,PathSize;
					BuildPath(InRoom->Data, HitActor->Data,PathCenter,PathSize);
			
					ABSPRoom* Room=World->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
						PathCenter,FRotator::ZeroRotator,ActorParams);
					PathCenter=PathCenter+PosAdjust;
					Room->InitRoom(FRoom(PathCenter,PathSize,bHorizontalSplit,true),RoomData);
					SpawnedPathways.Add(Room);
				}
			}
		}
		else if (Hit2.bBlockingHit)
		{
			if(Hit2.GetActor())
			{
				ABSPRoom* HitActor=Cast<ABSPRoom>(Hit1.GetActor());
				if(HitActor&&HitActor!=InPair[0]&&HitActor!=InPair[1]&&!HitActor->Data.bIsPathway)
				{

					FVector PathCenter,PathSize;
					BuildPath(InRoom->Data, HitActor->Data,PathCenter,PathSize);
			
					ABSPRoom* Room=World->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
						PathCenter,FRotator::ZeroRotator,ActorParams);
					PathCenter=PathCenter+NegAdjust;
					Room->InitRoom(FRoom(PathCenter,PathSize,bHorizontalSplit,true),RoomData);
					SpawnedPathways.Add(Room);
				}
			}
		}
			
		}
	
	

	
	
}








