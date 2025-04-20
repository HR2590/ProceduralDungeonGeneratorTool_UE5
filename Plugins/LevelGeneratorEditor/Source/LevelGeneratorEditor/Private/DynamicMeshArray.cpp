#include "DynamicMeshArray.h"

#include "PropertyCustomizationHelpers.h"

void SDynamicMeshArray::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Internal Meshes"))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
		]
		// Lista de elementos 
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(ListViewWidget, SListView<TSharedPtr<FDynamicMeshEntry>>)
			.ItemHeight(40)
			.ListItemsSource(&StaticMeshEntries)
			.OnGenerateRow(this, &SDynamicMeshArray::GenerateMeshRow)
		]
		+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoHeight()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("Add New"))
				.OnClicked(this, &SDynamicMeshArray::OnAddNewMesh)
			]
	];
}

TSharedRef<ITableRow> SDynamicMeshArray::GenerateMeshRow(TSharedPtr<FDynamicMeshEntry> Item,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FDynamicMeshEntry>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			//  Selector de Static Mesh
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SObjectPropertyEntryBox)
				.AllowedClass(UStaticMesh::StaticClass())
				.DisplayCompactSize(true)
				.DisplayThumbnail(true)
				.ObjectPath_Lambda([Item]() { return Item->Mesh ? Item->Mesh->GetPathName() : FString(); }) 
				.OnObjectChanged(this, &SDynamicMeshArray::OnMeshSelected, Item)
			]
	
			
			+ SHorizontalBox::Slot()
		   .AutoWidth()
		   .Padding(5)
		   [
			   SNew(SButton)
			   .Text(FText::FromString("-"))
			   .OnClicked(this, &SDynamicMeshArray::OnRemoveMesh, Item)
		   ]
		];
	
}

void SDynamicMeshArray::OnMeshSelected(const FAssetData& AssetData, TSharedPtr<FDynamicMeshEntry> Item)
{
	if (Item.IsValid())
	{
		Item->Mesh = Cast<UStaticMesh>(AssetData.GetAsset());
		ListViewWidget->RequestListRefresh(); 
	}
}

FReply SDynamicMeshArray::OnAddNewMesh()
{
		StaticMeshEntries.Add(MakeShareable(new FDynamicMeshEntry()));
		ListViewWidget->RequestListRefresh();
		return FReply::Handled();
	
}

FReply SDynamicMeshArray::OnRemoveMesh(TSharedPtr<FDynamicMeshEntry> Item)
{
	StaticMeshEntries.Remove(Item);
	ListViewWidget->RequestListRefresh();
	return FReply::Handled();
}

