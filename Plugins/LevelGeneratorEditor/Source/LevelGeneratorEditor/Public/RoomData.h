#pragma once
#include "MeshEntry.h"

struct
FRoomData
{
	TSharedPtr<FVector> InitPosition;
	TSharedPtr<uint32> width, height,minRoomSizeX,minRoomSizeY,minMeshSizeX,minMeshSizeY,minBaseRoomSizeX,minBaseRoomSizeY;
	TSharedPtr<uint32> minToleranceX,minToleranceY,DivisorX,DivisorY,Depth;
	TArray<TSharedPtr<FMeshEntry>> MeshEntries;
	
	FRoomData()
	{
		InitPosition=MakeShared<FVector>(FVector::ZeroVector);
		width= MakeShared<uint32>(2000);
		height= MakeShared<uint32>(2000);
		minRoomSizeX= MakeShared<uint32>(200);
		minRoomSizeY= MakeShared<uint32>(200);
		minMeshSizeX= MakeShared<uint32>(100);
		minMeshSizeY= MakeShared<uint32>(100);
		minBaseRoomSizeX=MakeShared<uint32>(*minMeshSizeX*(*minRoomSizeX));
		minBaseRoomSizeY=MakeShared<uint32>(*minMeshSizeY*(*minRoomSizeY));
		minToleranceX=MakeShared<uint32>(100);
		minToleranceY=MakeShared<uint32>(100);
		DivisorX=MakeShared<uint32>(1);
		DivisorY=MakeShared<uint32>(1);
		Depth=MakeShared<uint32>(4);
		MeshEntries.Add(MakeShared<FMeshEntry>(FMeshEntry(TEXT("FloorMesh"))));
		MeshEntries.Add(MakeShared<FMeshEntry>(FMeshEntry(TEXT("WallMesh"))));

	}
   TSharedPtr<uint32>& UpdateMinRoomSizeX()
   {
		*minBaseRoomSizeX=*minMeshSizeX**minRoomSizeX;
		return minRoomSizeX;
	}
   TSharedPtr<uint32>& UpdateMinRoomSizeY()
	{
		*minBaseRoomSizeY=*minMeshSizeY**minRoomSizeY;
		return minRoomSizeY;
	}
};
