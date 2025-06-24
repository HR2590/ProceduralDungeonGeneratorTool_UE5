// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPDungeonGenerator.h"
#include "BSPRoom.h"
#include "PCGComponent.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "PCGRoom.h"
#include "StaticMeshEditorSubsystem.h"
#include "StaticMeshEditorSubsystemHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


UBSPDungeonGenerator::UBSPDungeonGenerator()
{
}

void UBSPDungeonGenerator::bSplit(bool& bInHorizontalSplit,const FVector& InCenter, const FVector& InSize,
                                  FVector& Size_Part1, FVector& Center_Part1,FVector& Size_Part2,FVector& Center_Part2)
{
	float Difference;
	bInHorizontalSplit=!(InSize.X>InSize.Y);
	if (InSize.X>InSize.Y)
	{
		
		Difference=InSize.X/2;
		Size_Part1=FVector(Difference-DungeonData.tolerance.X/2,InSize.Y,InSize.Z);
		Size_Part2=FVector(Difference-DungeonData.tolerance.X/2,InSize.Y,InSize.Z);
			
		Center_Part1=FVector(InCenter.X-Difference/2-DungeonData.tolerance.X/2,InCenter.Y,InCenter.Z);
		Center_Part2=FVector(InCenter.X+Difference/2+DungeonData.tolerance.X/2,InCenter.Y,InCenter.Z);
		
		
	}
	else
	{
		Difference=InSize.Y/2;
		Size_Part1=FVector(InSize.X,Difference-DungeonData.tolerance.Y/2,InSize.Z);
		Size_Part2=FVector(InSize.X,Difference-DungeonData.tolerance.Y/2,InSize.Z);
			
		Center_Part1=FVector(InCenter.X,InCenter.Y-Difference/2-DungeonData.tolerance.Y/2,InCenter.Z);
		Center_Part2=FVector(InCenter.X,InCenter.Y+Difference/2+DungeonData.tolerance.Y/2,InCenter.Z);
		
		
	
	}

NormalizeVectorToSnap(Size_Part1,false);
NormalizeVectorToSnap(Size_Part2,false);

	NormalizeVectorToSnap(Center_Part1,true);
	NormalizeVectorToSnap(Center_Part2,true);
	
}


void UBSPDungeonGenerator::MeshAlgorithmSpace(const FVector& InCenter, const FVector& InSize, const int32& InDepth,int32& InSave)
{
	const int32 ActualDepth=InDepth;
	
	int32 SaveLeft=0;
	int32 SaveRight=SaveLeft;

	bool bSplitState=false;
	
	FVector Size_Part1 = FVector(InSize.X/2.0f,InSize.Y/2.0f,InSize.Z );
	FVector Size_Part2 = Size_Part1;
	
	FVector Center_Part1 = InCenter;
	FVector Center_Part2 = Center_Part1;

	const FInt32Vector Size=FInt32Vector(InSize);

	const FInt32Vector minRoomSize=FInt32Vector(DungeonData.minRoomSize);
	
	if (Size.X>=minRoomSize.X&&Size.Y>=minRoomSize.Y&&Size.X>0&&Size.Y>0)
	{
		bSplit(bSplitState,InCenter, InSize, Size_Part1, Center_Part1, Size_Part2, Center_Part2);
	}
	else
	{
		InSave=3;
		return;
	}
	
	
	
	MeshAlgorithmSpace(Center_Part1, Size_Part1, ActualDepth+1,SaveLeft);
	MeshAlgorithmSpace(Center_Part2, Size_Part2, ActualDepth+1,SaveRight);
	if(SaveLeft==3&&SaveRight==3) InSave=1;
	if(SaveLeft==1||SaveRight==1) InSave=1;
	
	if(SaveLeft==3&&SaveRight==3)
	{
		TArray<FRoom> ActualRooms;
		ActualRooms.Add(FRoom(InCenter, InSize,bSplitState,false,true));
		for (auto& InRoom : ActualRooms)
		{
			
			FActorSpawnParameters ObjectParams;
			ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
			ObjectParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
				InRoom.CenterPosition,FRotator::ZeroRotator,ObjectParams);
			Room->InitRoom(InRoom,DungeonData);

			SpawnedRooms.AddUnique(Room);
		}
		
		
	}
}

void UBSPDungeonGenerator::PartitionSpaceRandom(const FVector& InCenter, const FVector& InSize,const int32& InDepth,int32& InSave)
{
	TArray<FRoom> ActualRooms;
	
	const int32 ActualDepth=InDepth;
	
	int32 SaveLeft=0;
	int32 SaveRight=SaveLeft;

	bool bSplitState=false;
	
	FVector Size_Part1 = FVector(InSize.X/2,InSize.Y/2,InSize.Z );
	FVector Size_Part2 = Size_Part1;
	
	FVector Center_Part1 = InCenter;
	FVector Center_Part2 = Center_Part1;
	
	int32 SizeX=static_cast<int32>(InSize.X);
	int32 SizeY=static_cast<int32>(InSize.Y);
	
	if (ActualDepth<=DungeonData.algorithmDepth&&
		SizeX>=DungeonData.minRoomSize.X&&
		SizeY>=DungeonData.minRoomSize.X&&
		!FMath::IsNegativeOrNegativeZero(InSize.X)&&!FMath::IsNegativeOrNegativeZero(InSize.Y))
	{
		float minRoomDivider,Difference,Divider,SizeUnit2;

		if (SizeX>SizeY)
		{
			
			minRoomDivider=DungeonData.minRoomSize.X+DungeonData.tolerance.X;
			Difference= InSize.X-minRoomDivider;
			Divider = FMath::RoundToInt32(FMath::RandRange(minRoomDivider,Difference)/(DungeonData.minRoomMeshSize.X))*DungeonData.minRoomMeshSize.X;
			SizeUnit2=FMath::RoundToInt32(InSize.X-Divider);

			
			Size_Part1=FVector(Divider-DungeonData.tolerance.X,InSize.Y,InSize.Z);
			Size_Part2=FVector(SizeUnit2-DungeonData.tolerance.X,InSize.Y,InSize.Z);
			
			Center_Part1=FVector(InCenter.X-SizeUnit2/2,InCenter.Y,InCenter.Z);
			Center_Part2=FVector(InCenter.X+Divider/2,InCenter.Y,InCenter.Z);
		}
		else
		{
			bSplitState=true;
			minRoomDivider=DungeonData.minRoomSize.Y+DungeonData.tolerance.Y;
			Difference=InSize.Y-minRoomDivider;
			Divider = FMath::RoundToInt32(FMath::RandRange(minRoomDivider, Difference)/(DungeonData.minRoomMeshSize.Y))*DungeonData.minRoomMeshSize.Y;
			SizeUnit2=FMath::RoundToInt32(InSize.Y-Divider);
			
			Size_Part1=FVector(InSize.X,Divider-DungeonData.tolerance.Y,InSize.Z);
			Size_Part2=FVector(InSize.X,SizeUnit2-DungeonData.tolerance.Y,InSize.Z);
			
			Center_Part1=FVector(InCenter.X,InCenter.Y-SizeUnit2/2,InCenter.Z);
			Center_Part2=FVector(InCenter.X,InCenter.Y+Divider/2,InCenter.Z);
		}

	}
	else
	{
		InSave=3;
		return;
	}
	
	//NormalizeVectorToSnap(Center_Part1,false);
	//NormalizeVectorToSnap(Center_Part2,false);
	
	PartitionSpaceRandom(Center_Part1, Size_Part1, ActualDepth+1,SaveLeft);
	PartitionSpaceRandom(Center_Part2, Size_Part2, ActualDepth+1,SaveRight);
	if(SaveLeft==3&&SaveRight==3) InSave=1;
	if(SaveLeft==1) ActualRooms.Add(FRoom(Center_Part1, Size_Part1,bSplitState,false,false));
	if(SaveRight==1) ActualRooms.Add(FRoom(Center_Part2, Size_Part2,bSplitState,false,true));

	if(ActualRooms.Num()!=0)
	{
		TArray<ABSPRoom*> PairRoom;

		
		for (auto& InRoom : ActualRooms)
		{
			
			FActorSpawnParameters ObjectParams;
			ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
			ObjectParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			ABSPRoom* Room=GEditor->GetEditorWorldContext().World()->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
				InRoom.CenterPosition,FRotator::ZeroRotator,ObjectParams);
			Room->InitRoom(InRoom,DungeonData);
			
			SpawnedRooms.AddUnique(Room);
		}
		
		
	}
	
}


void UBSPDungeonGenerator::RespawnPlayerStart()
{
	for (auto& Room : SpawnedRooms)
	{
			FActorSpawnParameters ObjectParams;
			ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
			if(Room->Data.BridgeActors.Num()>0)
			{
				GEditor->GetEditorWorldContext().World()->SpawnActor<APlayerStart>(
					APlayerStart::StaticClass(),
					Room->Data.CenterPosition+
					FVector(0,0,DungeonData.minRoomMeshSize.Z),
					FRotator::ZeroRotator, ObjectParams);
				break;
			}
		
	}
}

void UBSPDungeonGenerator::NormalizeVectorToSnap(FVector& InVector,bool IsPathway) const
{
	FInt32Vector VectorSnap=FInt32Vector(InVector);
	if(IsPathway)
	{
		InVector.X=FMath::RoundToInt32(VectorSnap.X/DungeonData.minRoomMeshSize.X)*DungeonData.minRoomMeshSize.X;
		InVector.Y=FMath::RoundToInt32(VectorSnap.Y/DungeonData.minRoomMeshSize.Y)*DungeonData.minRoomMeshSize.Y;
		InVector.Z=FMath::RoundToInt32(VectorSnap.Z/DungeonData.minRoomMeshSize.Z)*DungeonData.minRoomMeshSize.Z;
	}
	else
	{
		InVector.X=FMath::TruncToInt32(VectorSnap.X/DungeonData.minRoomMeshSize.X)*DungeonData.minRoomMeshSize.X;
		InVector.Y=FMath::TruncToInt32(VectorSnap.Y/DungeonData.minRoomMeshSize.Y)*DungeonData.minRoomMeshSize.Y;
		InVector.Z=FMath::TruncToInt32(VectorSnap.Z/DungeonData.minRoomMeshSize.Z)*DungeonData.minRoomMeshSize.Z;
	}
	

	
}

bool UBSPDungeonGenerator::IsCentered(const int32& InNumber)
{
	
	
	//UE_LOG(LogTemp, Warning, TEXT("Size: %d"), InNumber);
	if(InNumber%2!=0) return true;
	
	int32 firstDigit=InNumber;

	while(true)
	{
		
		if(firstDigit==0)break;
		if(firstDigit%10!=0)break;
		
		firstDigit/=10;
	}

	
	//UE_LOG(LogTemp, Warning, TEXT("digit: %d"), firstDigit);
	//UE_LOG(LogTemp, Warning, TEXT("result: %s"), (LastValue % 2 == 1) ? TEXT("true") : TEXT("false"));

	return firstDigit % 2 != 0;

	
}

void UBSPDungeonGenerator::SaveLoadingWidget(UEditorUtilityWidgetBlueprint* InWidget)
{
	if(InWidget) Widget=InWidget;
}

void UBSPDungeonGenerator::UpdateChestClass(TArray<TSubclassOf<AActor>> InClass)
{
	if(InClass.IsEmpty()) DeleteChests();
	
	DungeonData.ChestActors=InClass;
}

void UBSPDungeonGenerator::UpdatePCGClass(TArray<TSubclassOf<AActor>> InClass)
{
	if(InClass.IsEmpty()) DeleteInternals();
	DungeonData.PCGActors=InClass;
}


void UBSPDungeonGenerator::BSPMeshAlgorithm(const FDungeonData& InRoomData)
{
	LoadingBar();
	
	int32 Save=0;
	int32 Depth=0;
	DungeonData = InRoomData;
	DeleteDungeon();

	if(DungeonData.floorMeshRoom.IsEmpty()||DungeonData.wallMeshRoom.IsEmpty()||
	   DungeonData.floorMeshPathway.IsEmpty()||DungeonData.wallMeshPathway.IsEmpty()) return;


	MeshAlgorithmSpace(FVector(DungeonData.initPosition),
	                      FVector(DungeonData.dimensions.X, DungeonData.dimensions.Y, 1), Depth, Save);
	GeneratePathways(DungeonData,false);
	RespawnPlayerStart();
	FSlateApplication::Get().ReleaseAllPointerCapture();
}

void UBSPDungeonGenerator::BSPRandomAlgorithm(const FDungeonData& InRoomData)
{
	LoadingBar();
	int32 Save=0;
	DungeonData = InRoomData;
	DeleteDungeon();

	if(DungeonData.floorMeshRoom.IsEmpty()||DungeonData.wallMeshRoom.IsEmpty()||
	   DungeonData.floorMeshPathway.IsEmpty()||DungeonData.wallMeshPathway.IsEmpty()) return;
	
	PartitionSpaceRandom(FVector(DungeonData.initPosition), FVector(DungeonData.dimensions.X, DungeonData.dimensions.Y,1), 0,Save);

	GeneratePathways(DungeonData,false);

	RespawnPlayerStart();
	
	FSlateApplication::Get().ReleaseAllPointerCapture();

}

void UBSPDungeonGenerator::DestroyRoomsFromArray(TArray<ABSPRoom*>& InArrayRooms)
{
	if(InArrayRooms.IsEmpty()) return;
	
	for (auto& ActorRoom : InArrayRooms)
		if(ActorRoom&&!ActorRoom->IsPendingKillPending())
		{
			ActorRoom->Destroy(false,true);
		}
		
}

void UBSPDungeonGenerator::DeleteDungeon()
{
	LoadingBar();
	DeleteRooms();
	DeletePathways();
	DeleteInternals();
	DeleteChests();
	
}

void UBSPDungeonGenerator::DeleteRooms()
{
	LoadingBar();
	DeleteAllPlayerStarts();
	
	if(SpawnedRooms.IsEmpty()) return;
	
	DestroyRoomsFromArray(SpawnedRooms);
	
	SpawnedRooms.Empty();
}

void UBSPDungeonGenerator::DeletePathways()
{
	LoadingBar();
	DestroyRoomsFromArray( SpawnedPathways);
		SpawnedPathways.Empty();
	
	ClearDoors(true,true);
}

void UBSPDungeonGenerator::DeleteInternals()
{
	LoadingBar();
	if(SpawnedPCG.IsEmpty()) return;
	for (auto& PCG : SpawnedPCG)
	{
		if(PCG) PCG->Destroy();
	}
	
	
}

void UBSPDungeonGenerator::DeleteAllPlayerStarts()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(),APlayerStart::StaticClass(),PlayerStarts);

	for (auto& PlayerStart : PlayerStarts)
	{
		PlayerStart->Destroy();
	}
}


void UBSPDungeonGenerator::BuildPath(const FVector& FirstHit, const FVector& SecondHit,FVector& InCenter,FVector& InSize)
{
	FInt32Vector Hit1=FInt32Vector(FirstHit);
	FInt32Vector Hit2=FInt32Vector(SecondHit);
	
	if(FMath::IsNearlyEqual(Hit1.X,Hit2.X,2.0f))
	{
		if(Hit1.Y>Hit2.Y)
			InSize = FVector(DungeonData.minPathwaySize.X,FirstHit.Y-SecondHit.Y,DungeonData.minPathwaySize.Z);
		else
			InSize = FVector(DungeonData.minPathwaySize.X,SecondHit.Y-FirstHit.Y,DungeonData.minPathwaySize.Z);
	}
	else
	{
		if(Hit1.X>Hit2.X)
			InSize = FVector(FirstHit.X-SecondHit.X,DungeonData.minPathwaySize.Y,DungeonData.minPathwaySize.Z);
		else
			InSize = FVector(SecondHit.X-FirstHit.X,DungeonData.minPathwaySize.Y,DungeonData.minPathwaySize.Z);
	}
	InCenter=(FirstHit+SecondHit)/2;
	InCenter=InCenter-FVector(0,0,InCenter.Z);
	//NormalizeVectorToSnap(InCenter,true);
	
}




void UBSPDungeonGenerator::GeneratePathways(const FDungeonData& InRoomData,bool RandomPaths)
{
	LoadingBar();
	DeletePathways();
	
	DungeonData=InRoomData;
	for (auto& Room : SpawnedRooms)
		 if(Room) RandomTraceLine(Room,RandomPaths);
}




void UBSPDungeonGenerator::RandomTraceLine(ABSPRoom* InRoom,bool RandomPaths)
{
	int MaxIndex=4;
	int Index;
	float LimitX=DungeonData.initPosition.X+DungeonData.dimensions.X;
	float LimitY=DungeonData.initPosition.Y+DungeonData.dimensions.Y;
	
	FVector ZIncrement=FVector(0,0,DungeonData.minRoomMeshSize.Z);
	
	UWorld* World=GEditor->GetEditorWorldContext().World();
	bool bHorizontalSplit=false;
	
	FHitResult Hit,Hit1,Hit2,UpperDoor;
	FHitResult HitUpperLimit,HitLowerLimit;
	FHitResult InvHit,InvUpperDoor;
	
	FCollisionObjectQueryParams ObjectParams;
	FActorSpawnParameters ActorParams;
	
	ActorParams.ObjectFlags=RF_Transactional|RF_Public;
	ActorParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	if(RandomPaths) MaxIndex=FMath::RandRange(1,4);

	
	FVector CenterOffset=FVector::Zero();
	bool XAdjusted,YAdjusted;
	YAdjusted=XAdjusted=false;
	
	if(!IsCentered(FMath::RoundToInt32(InRoom->Data.Size.X/DungeonData.minRoomMeshSize.X)))
	{
		CenterOffset=CenterOffset+FVector(DungeonData.minRoomMeshSize.X/2,0,0);
		XAdjusted=true;
	}
	
	if(!IsCentered(FMath::RoundToInt32(InRoom->Data.Size.Y/DungeonData.minRoomMeshSize.Y)))
	{
		CenterOffset=CenterOffset+FVector(0,DungeonData.minRoomMeshSize.Y/2,0);
		YAdjusted=true;
	}
	

	CenterOffset=CenterOffset+InRoom->Data.CenterPosition+FVector(0,0,DungeonData.minRoomMeshSize.Z/2);
	
	for(SIZE_T i=1;i<=MaxIndex;i++)
		{
			FCollisionQueryParams QueryParams;
			FVector End,PosAdjust,NegAdjust,InvAdjust;
			FVector LimitAdjust;
		
			FVector Start=CenterOffset;
				
		
			Hit.Reset();Hit1.Reset();Hit2.Reset();HitUpperLimit.Reset();HitLowerLimit.Reset();

			if(RandomPaths) Index=FMath::RandRange(1,4);
			else Index=i;
		
			switch (Index)
			{
			case 1:
				PosAdjust=FVector(0,DungeonData.minRoomMeshSize.Y/2,0);
				Start=Start+FVector(InRoom->Data.Size.X/2,0,0)+FVector(DungeonData.minRoomMeshSize.X/2,0,0)*!XAdjusted;
				LimitAdjust=FVector(0,DungeonData.minPathwaySize.Y/2,0)-PosAdjust;
				End=FVector(FMath::Clamp(Start.X+DungeonData.dimensions.X,LimitX*-1,LimitX),Start.Y,Start.Z);
				InvAdjust=FVector(DungeonData.minRoomMeshSize.X,0,0)*-1;
				bHorizontalSplit=false;
				break;
			case 2:
				PosAdjust=FVector(0,DungeonData.minRoomMeshSize.Y/2,0);
				Start=Start-FVector(InRoom->Data.Size.X/2,0,0)-FVector(DungeonData.minRoomMeshSize.X/2,0,0)*(2*XAdjusted);
				LimitAdjust=FVector(0,DungeonData.minPathwaySize.Y/2,0)-PosAdjust;
				End=FVector(FMath::Clamp(Start.X-DungeonData.dimensions.X,LimitX*-1,LimitX),Start.Y,Start.Z);
				InvAdjust=FVector(DungeonData.minRoomMeshSize.X,0,0);
				bHorizontalSplit=false;
				break;
			case 3:
				PosAdjust=FVector(DungeonData.minRoomMeshSize.X/2,0,0);
				Start=Start+FVector(0,InRoom->Data.Size.Y/2,0)+FVector(0,DungeonData.minRoomMeshSize.Y/2,0)*!YAdjusted;
				LimitAdjust=FVector(DungeonData.minPathwaySize.X/2,0,0)-PosAdjust;
				End=FVector(Start.X,FMath::Clamp(Start.Y+DungeonData.dimensions.Y,LimitY*-1,LimitY),Start.Z);
				InvAdjust=FVector(0,DungeonData.minRoomMeshSize.Y,0)*-1;
				bHorizontalSplit=true;
				break;
			case 4:
				PosAdjust=FVector(DungeonData.minRoomMeshSize.X/2,0,0);
				Start=Start-FVector(0,InRoom->Data.Size.Y/2,0)-FVector(0,DungeonData.minRoomMeshSize.Y/2,0)*(2*XAdjusted);
				LimitAdjust=FVector(DungeonData.minPathwaySize.X/2,0,0)-PosAdjust;
				End=FVector(Start.X,FMath::Clamp(Start.Y-DungeonData.dimensions.Y,LimitY*-1,LimitY),Start.Z);
				InvAdjust=FVector(0,DungeonData.minRoomMeshSize.Y,0);
				bHorizontalSplit=true;
				break;
			default:
				break;
			}

			NegAdjust=PosAdjust*-1;
			// Line Trace Door and limits
			//World->LineTraceSingleByObjectType(Hit1,Start+PosAdjust,End+PosAdjust,ObjectParams,QueryParams);
			World->LineTraceSingleByObjectType(Hit,Start,End,ObjectParams,QueryParams);
			//World->LineTraceSingleByObjectType(Hit2,Start+NegAdjust,End+NegAdjust,ObjectParams,QueryParams);
			World->LineTraceSingleByObjectType(UpperDoor,Start+ZIncrement,End+ZIncrement,ObjectParams,QueryParams);
			//Line Trace Limits
			World->LineTraceSingleByObjectType(HitUpperLimit,Start+LimitAdjust,End+LimitAdjust,ObjectParams,QueryParams);
			World->LineTraceSingleByObjectType(HitLowerLimit,Start-LimitAdjust,End-LimitAdjust,ObjectParams,QueryParams);
			//Inv Raycasts
			World->LineTraceSingleByObjectType(InvHit,Start,Start+InvAdjust,ObjectParams,QueryParams);

			//Debug Lines
			//DrawDebugLine(World, Start+PosAdjust, End+PosAdjust, FColor::Green,false,60.0f, 0, 2.0f);
			//DrawDebugLine(World, Start, End, FColor::Green,false,20.0f, 0, 2.0f);
			//DrawDebugLine(World, Start+NegAdjust, End+NegAdjust, FColor::Green,false,60.0f, 0, 2.0f);

			//DrawDebugLine(World, Start+LimitAdjust, End+LimitAdjust, FColor::Green,false,5.0f, 0, 2.0f);
			//DrawDebugLine(World, Start-LimitAdjust, End-LimitAdjust, FColor::Green,false,5.0f, 0, 2.0f);

			
			//if (Hit.bBlockingHit&&Hit1.bBlockingHit&&Hit2.bBlockingHit&&HitUpperLimit.bBlockingHit&&HitLowerLimit.bBlockingHit)
			if (Hit.bBlockingHit&&HitUpperLimit.bBlockingHit&&HitLowerLimit.bBlockingHit&&InvHit.bBlockingHit)
			{
				
				if(Hit.GetActor()&&HitUpperLimit.GetActor()&&HitLowerLimit.GetActor()&&InvHit.GetActor())
				{
					//Parents
					auto HitParentUpperActor=Cast<ABSPRoom>(HitUpperLimit.GetActor()->GetAttachParentActor());
					auto HitParentLowerActor=Cast<ABSPRoom>(HitLowerLimit.GetActor()->GetAttachParentActor());
					auto HitParentActor=Cast<ABSPRoom>(Hit.GetActor()->GetAttachParentActor());
					auto InvParentActor=Cast<ABSPRoom>(InvHit.GetActor()->GetAttachParentActor());
					//Front& Inv Door
					auto HitDoor=Cast<AStaticMeshActor>(Hit.GetActor());
					auto InvDoor=Cast<AStaticMeshActor>(InvHit.GetActor());

					//Debug
					//DrawDebugPoint(World,Hit.ImpactPoint,5.0f,FColor::Red,false,20.f,0);
					//DrawDebugPoint(World,Hit1.ImpactPoint,5.0f,FColor::Red,true,-1,0);
					//DrawDebugPoint(World,Hit2.ImpactPoint,5.0f,FColor::Red,true,-1,0);
					
					//DrawDebugLine(World, Start+ZIncrement, Start+InvAdjust, FColor::Green,false,5.0f, 0, 2.0f);
					//DrawDebugLine(World, Start+ZIncrement, Start+InvAdjust+ZIncrement, FColor::Green,false,5.0f, 0, 2.0f);
					//UE_LOG(LogTemp,Warning,TEXT("ActorInv:%d"),InvHit.bBlockingHit&&InvHit.GetActor());
					
					if (HitDoor&&!HitParentActor->Data.bIsPathway&&!HitParentActor->Data.BridgeActors.Contains(InRoom)&&
					//if (HitDoor&&!HitParentActor->Data.BridgeActors.Contains(InRoom)&&
						HitParentActor==HitParentUpperActor&&HitParentUpperActor==HitParentLowerActor)
					{
						
						World->LineTraceSingleByObjectType(InvUpperDoor,Start+ZIncrement,Start+InvAdjust+ZIncrement,ObjectParams,QueryParams);
						//if(true)
						if (FMath::IsNearlyEqual(InvDoor->GetActorLocation().X,HitDoor->GetActorLocation().X,2.0f)
						||FMath::IsNearlyEqual(InvDoor->GetActorLocation().Y,HitDoor->GetActorLocation().Y,2.0f))
						{
							
							if (InvParentActor&&InvParentActor==InRoom)
							{
								FInt32Vector Difference=FInt32Vector(InvDoor->GetActorLocation()-HitDoor->GetActorLocation());
								FInt32Vector MinLimit=FInt32Vector(DungeonData.minPathwaySize);

								//UE_LOG(LogTemp,Warning,TEXT("Difference:%s"),*Difference.ToString());
								//UE_LOG(LogTemp,Warning,TEXT("MinRoomSize:%s"),*DungeonData.minRoomSize.ToString());

								
								if(FMath::Abs(Difference.X)>=MinLimit.X||FMath::Abs(Difference.Y)>=MinLimit.Y)
									//||FMath::IsNearlyEqual(FMath::Abs(Difference.X),MinLimit.X,1e-01)
									//||FMath::IsNearlyEqual(FMath::Abs(Difference.Y),MinLimit.Y,1e-01))
									
									//&&(FMath::Abs(Difference.X)%FMath::RoundToInt32(DungeonData.minRoomMeshSize.X)==0
									//&&FMath::Abs(Difference.Y)%FMath::RoundToInt32(DungeonData.minRoomMeshSize.Y)==0))
								{

									FVector DifferenceAdjust=FVector(Difference.X/DungeonData.minRoomMeshSize.X,
									Difference.Y/DungeonData.minRoomMeshSize.Y,0);
									FVector RoundAdjust=FVector(FMath::RoundToInt32(DifferenceAdjust.X),
									FMath::RoundToInt32(DifferenceAdjust.Y),0);
									DifferenceAdjust=(DifferenceAdjust-RoundAdjust)*DungeonData.minRoomMeshSize;
									//UE_LOG(LogTemp,Warning,TEXT("DiffAdjust:%s"),*DifferenceAdjust.ToString());

									HitParentActor->SetActorLocation(HitParentActor->GetActorLocation()+DifferenceAdjust);
									FVector PathCenter,PathSize;
									HitParentActor->Data.BridgeActors.AddUnique(InRoom);
									InRoom->Data.BridgeActors.AddUnique(HitParentActor);
									
									BuildPath(InvDoor->GetActorLocation(), HitDoor->GetActorLocation(),PathCenter,PathSize);
								
									ABSPRoom* Room=World->SpawnActor<ABSPRoom>(ABSPRoom::StaticClass(),
										PathCenter,FRotator::ZeroRotator,ActorParams);
								
									//UE_LOG(LogTemp,Warning,TEXT("Hit Door:%s"),*(HitDoor->GetActorLocation()).ToString());
								
									HitParentActor->Data.Doors.AddUnique(HitDoor);
									HitDoor->GetStaticMeshComponent()->SetVisibility(false);
									HitDoor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
									//UE_LOG(LogTemp,Warning,TEXT("Inv Door:%s"),*(InvDoor->GetActorLocation()).ToString());
								
									InvParentActor->Data.Doors.AddUnique(InvDoor);
									InvDoor->GetStaticMeshComponent()->SetVisibility(false);
									InvDoor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
									if(UpperDoor.GetActor())
									{
										HitParentActor->Data.Doors.AddUnique(Cast<AStaticMeshActor>(UpperDoor.GetActor()));
										Cast<AStaticMeshActor>(UpperDoor.GetActor())->GetStaticMeshComponent()->SetVisibility(false);
										Cast<AStaticMeshActor>(UpperDoor.GetActor())->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
									}
								
									if(InvUpperDoor.GetActor())
									{
										InvParentActor->Data.Doors.AddUnique(Cast<AStaticMeshActor>(InvUpperDoor.GetActor()));
										Cast<AStaticMeshActor>(InvUpperDoor.GetActor())->GetStaticMeshComponent()->SetVisibility(false);
										Cast<AStaticMeshActor>(InvUpperDoor.GetActor())->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
									}
								
									Room->InitRoom(FRoom(PathCenter,PathSize,bHorizontalSplit,true,false),DungeonData);
									SpawnedPathways.AddUnique(Room);
									
								}
								
								
							}
						}
						
						
					}
				}
			}

			//if(i==4&&InRoom&&InRoom->Data.BridgeActors.IsEmpty()) InRoom->Destroy();
		
		}
	
	

	
}

void UBSPDungeonGenerator::DeleteNoPathwayRooms()
{
	LoadingBar();
	for (auto& Room : SpawnedRooms)
	{
		if(Room&&!Room->IsPendingKillPending()&&Room->Data.BridgeActors.IsEmpty())
			Room->Destroy();
	}
	
}


void UBSPDungeonGenerator::ClearDoors(bool InVisibility,bool InClearAll)
{
	for (auto Room : SpawnedRooms)
	{
			if(Room&&!Room->Data.Doors.IsEmpty())
			{
				for (auto InDoor : Room->Data.Doors)
				{
					if(InDoor&&!InDoor->IsPendingKillPending())
					{
						InDoor->GetStaticMeshComponent()->SetVisibility(InVisibility,false);
						InDoor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("BlockAll"));
					}
				}
					
				if(InClearAll)
					Room->Data.BridgeActors.Empty();
			}
		
		
		
	}
}

void UBSPDungeonGenerator::PackageAll()
{
	LoadingBar();
	TArray<AStaticMeshActor*> AllActors;
	if(SpawnedRooms.IsEmpty()) return;
	
	for (auto& Room : SpawnedRooms)
	{
		if(Room&&!Room->Data.Doors.IsEmpty())
		{
			for (auto Door :Room->Data.Doors )
			{
				if(Door&&!Door->IsPendingKillPending())
					Door->Destroy();
			}
		}
	}
	for (auto& Room : SpawnedRooms)
	{
		if(!Room) continue;
		AllActors.Add(MergeStaticMeshes(Room->Data.Floors));
		AllActors.Add(MergeStaticMeshes(Room->Data.LeftWalls));
		AllActors.Add(MergeStaticMeshes(Room->Data.RightWalls));
		AllActors.Add(MergeStaticMeshes(Room->Data.UpWalls));
		AllActors.Add(MergeStaticMeshes(Room->Data.DownWalls));
		
	}
	for (auto& Pathway : SpawnedPathways)
	{
		if(!Pathway) continue;
		AllActors.Add(MergeStaticMeshes(Pathway->Data.Floors));
		AllActors.Add(MergeStaticMeshes(Pathway->Data.LeftWalls));
		AllActors.Add(MergeStaticMeshes(Pathway->Data.RightWalls));
		AllActors.Add(MergeStaticMeshes(Pathway->Data.UpWalls));
		AllActors.Add(MergeStaticMeshes(Pathway->Data.DownWalls));
	}
	AStaticMeshActor* DungeonMesh=MergeStaticMeshes(AllActors);
	DungeonMesh->SetPivotOffset(DungeonData.initPosition);
	DungeonMesh->SetActorLocation(DungeonData.initPosition);
	
	/*
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
	}*/
	
	DeleteDungeon();

}

void UBSPDungeonGenerator::SpawnPCG()
{
	DeleteInternals();
	if(DungeonData.PCGActors.IsEmpty()) return;
	
	FActorSpawnParameters ObjectParams;
	ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
	ObjectParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (auto InRoom : SpawnedRooms)
	{
		TSubclassOf<AActor> RandomClass=DungeonData.PCGActors[FMath::RandRange(0,DungeonData.PCGActors.Num()-1)];
		AActor* PCGActor=GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(RandomClass,
			InRoom->Data.CenterPosition,FRotator::ZeroRotator,ObjectParams);
	
		TSubclassOf<UPCGComponent> CompClass= UPCGComponent::StaticClass();
	
		APCGRoom* PCGRoom=Cast<APCGRoom>(PCGActor);
		
		//UE_LOG(LogTemp, Warning, TEXT("Spawned actor class: %s"), *PCGRoom->GetClass()->GetName());

		TArray<UActorComponent*> PCGComponents=PCGActor->K2_GetComponentsByClass(CompClass);
	
		for(auto Comps : PCGComponents)
		{
			if(auto PCGComp=Cast<UPCGComponent>(Comps))
			{
				FString FunctionCall = FString::Printf(TEXT("PrintTest"));
				PCGActor->CallFunctionByNameWithArguments(*FunctionCall, *GLog, nullptr, true);
				//PCGRoom->SetColumnsFloatParameter(TEXT("DistanceX"),InRoom->Data.Size.X/10);
				//PCGRoom->SetColumnsFloatParameter(TEXT("DistanceY"),InRoom->Data.Size.Y/10);
				PCGComp->Generate();
			}
			
		}
	
	
		SpawnedPCG.Add(PCGActor);
	}
	
}

AStaticMeshActor* UBSPDungeonGenerator::MergeStaticMeshes(const TArray<AStaticMeshActor*>& StaticMeshes)
{
	if (StaticMeshes.Num() == 0) return nullptr;
	
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

void UBSPDungeonGenerator::LoadingBar()
{
#if WITH_EDITOR
	if(!Widget||IsLoadingBarActive()) return;

	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GEditor->GetEditorWorldContext().World(), Cast<UClass>(Widget->GeneratedClass));
	
	if (!UserWidget) return;

	auto LoadingWidget = SNew(SWindow)
		.Title(FText::FromString(TEXT("DungeonGenerator")))
		.ClientSize(FVector2D(400.f, 200.f))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.HasCloseButton(true)
		.IsTopmostWindow(true);
	
	TSharedPtr<SWidget> WidgetContent = UserWidget->TakeWidget();
	
		LoadingWidget->SetContent(
			SNew(SBox)
			.WidthOverride(400.f)
			.HeightOverride(200.f)
			[
				WidgetContent.ToSharedRef()
			]
		);

	FSlateApplication::Get().AddWindow(LoadingWidget);
	FSlateApplication::Get().Tick();
	
#endif

}



void UBSPDungeonGenerator::DestroyLoadingBar()
{
	
	const TArray<TSharedRef<SWindow>>& Windows = FSlateApplication::Get().GetInteractiveTopLevelWindows();
	
	for (const TSharedRef<SWindow>& Window : Windows)
	{
		FString Title = Window->GetTitle().ToString();
		if(Title.Find(TEXT("DungeonGenerator"))>=0)
		{
			Window->DestroyWindowImmediately();
	
		}
	}
	FSlateApplication::Get().ReleaseAllPointerCapture();
}

bool UBSPDungeonGenerator::IsLoadingBarActive()
{
	const FString Title = FSlateApplication::Get().GetActiveTopLevelWindow()->GetTitle().ToString();
	if(Title.Find(TEXT("DungeonGenerator"))>=0)
	{
		return true;
	}
	return false;
}


void UBSPDungeonGenerator::SelectParentActor(AActor* InActor)
{
	if(!InActor) return;
	if (AActor* Parent = InActor->GetAttachParentActor())
	{
		GEditor->SelectNone(false, true);
		GEditor->SelectActor(Parent, true, true);
	}
}

void UBSPDungeonGenerator::SpawnChests()
{
	if(SpawnedRooms.IsEmpty()||DungeonData.ChestActors.IsEmpty()) return;
	DeleteChests();
	FActorSpawnParameters ObjectParams;
	ObjectParams.ObjectFlags=RF_Transactional|RF_Public;
	ObjectParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	int32 ChestNumber=FMath::RandRange(0,SpawnedRooms.Num()-1);
	
	for (int32 i=0;i<ChestNumber;i++)
	{
		
		
		FVector RandomRoomPosition=SpawnedRooms[FMath::RandRange(0,SpawnedRooms.Num()-1)]->GetActorLocation();
		AActor** Chest=SpawnedChests.FindByPredicate([RandomRoomPosition](const AActor* InActor)
				{
					FVector ChestPostion=InActor->GetActorLocation();
					return FMath::IsNearlyEqual(ChestPostion.X,RandomRoomPosition.X,1E-01)
					&& FMath::IsNearlyEqual(ChestPostion.Y,RandomRoomPosition.Y,1E-01)
					&& FMath::IsNearlyEqual(ChestPostion.Z,RandomRoomPosition.Z,1E-01);
				});
		if(!Chest)
		{
			TSubclassOf<AActor> RandomClass=DungeonData.ChestActors[FMath::RandRange(0,DungeonData.ChestActors.Num()-1)];
			AActor* ChestActor=GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(RandomClass,
				RandomRoomPosition,FRotator::ZeroRotator,ObjectParams);
			SpawnedChests.Add(ChestActor);
		}
	}
}

TArray<ABSPRoom*> UBSPDungeonGenerator::GetRooms()
{
	return  SpawnedRooms;
}



void UBSPDungeonGenerator::AddSpawnedPCG(AActor* InActor)
{
	if(!InActor) return;
	SpawnedPCG.Add(InActor);
}

void UBSPDungeonGenerator::DeleteChests()
{
	for(auto& Chest : SpawnedChests)
	{
		if(Chest&&!Chest->IsPendingKillPending()) Chest->Destroy();
	}
	SpawnedChests.Empty();
}











