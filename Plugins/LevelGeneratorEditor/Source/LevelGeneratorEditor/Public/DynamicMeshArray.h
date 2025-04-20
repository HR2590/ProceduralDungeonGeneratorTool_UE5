#pragma once


struct FDynamicMeshEntry
{
	TObjectPtr<UStaticMesh> Mesh;
};

class SDynamicMeshArray:public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDynamicMeshArray){}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
private:
	TArray<TSharedPtr<FDynamicMeshEntry>> StaticMeshEntries;
	TSharedPtr<SListView<TSharedPtr<FDynamicMeshEntry>>> ListViewWidget;
	TSharedRef<ITableRow> GenerateMeshRow(TSharedPtr<FDynamicMeshEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnMeshSelected(const FAssetData& AssetData, TSharedPtr<FDynamicMeshEntry> Item);
	FReply OnAddNewMesh();
	FReply OnRemoveMesh(TSharedPtr<FDynamicMeshEntry> Item);

};
