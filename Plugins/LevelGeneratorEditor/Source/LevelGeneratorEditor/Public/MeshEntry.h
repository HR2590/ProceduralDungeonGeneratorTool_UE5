#pragma once

#include "MeshEntry.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct FMeshEntry
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FString Name;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) UStaticMesh* Mesh;
	//TObjectPtr<UStaticMesh> Mesh;
	FMeshEntry(): Mesh(nullptr)
	{
	};
	FMeshEntry(const FString& InName) : Name(InName), Mesh(nullptr)
	{
	}
};
