#pragma once

struct FMeshEntry
{
	FString Name;
	TObjectPtr<UStaticMesh> Mesh;
	FMeshEntry(const FString& InName) : Name(InName), Mesh(nullptr) {}
};
