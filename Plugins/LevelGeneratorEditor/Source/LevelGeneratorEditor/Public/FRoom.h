#pragma once

#include "FRoom.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct FRoom
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FVector CenterPosition;
	UPROPERTY(EditAnywhere) FVector Size;
	UPROPERTY(EditAnywhere) TArray<bool> Walls;
	UPROPERTY(VisibleAnywhere) bool bHorizontalSplit;
	UPROPERTY(VisibleAnywhere) bool bIsPathway;
	

	
	FRoom(): CenterPosition(0), Size(0), bHorizontalSplit(false), bIsPathway(false)
	{
		for (uint32 i = 0; i < 4; i++) Walls.Add(true);
	}
	
	FRoom(const FVector& InPosition, const FVector& InSize,const bool& bInHorizontalSplit,const bool& bInIsPathway):
			CenterPosition(InPosition), Size(InSize),bHorizontalSplit(bInHorizontalSplit), bIsPathway(bInIsPathway)
	{
		if(!bIsPathway)
			for (uint32 i = 0; i < 4; i++) Walls.Add(true);
		else
		{
			if(bInHorizontalSplit)
			{
				Walls.Add(true);Walls.Add(true);
				Walls.Add(false);Walls.Add(false);
			}
			else
			{
				Walls.Add(false);Walls.Add(false);
				Walls.Add(true);Walls.Add(true);
			}
		}
	}
	
	bool operator==(const FRoom& Other) const
	{
		return CenterPosition == Other.CenterPosition && Size == Other.Size && bHorizontalSplit == Other.bHorizontalSplit;
	}
};
