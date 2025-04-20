
#include "StaticMeshArray.h"

#include "PropertyCustomizationHelpers.h"

void SStaticMeshArrayWidget::Construct(const FArguments& InArgs)
{
	// Inicializamos la lista con los elementos "Floor" y "Wall"
	//StaticMeshEntries.Add(MakeShareable(new FMeshEntry(TEXT("Floor"))));
	StaticMeshEntries=InArgs._MeshEntries;

	ChildSlot
	[
		SNew(SVerticalBox)
		// Lista de elementos 
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(ListViewWidget, SListView<TSharedPtr<FMeshEntry>>)
			.ItemHeight(40)
			.ListItemsSource(&StaticMeshEntries)
			.OnGenerateRow(this, &SStaticMeshArrayWidget::GenerateMeshRow)
		]
		
	];
}



TSharedRef<ITableRow> SStaticMeshArrayWidget::GenerateMeshRow(TSharedPtr<FMeshEntry> Item,
                                                              const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMeshEntry>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(STextBlock)
				.Text(FText::FromString(Item->Name))
			]

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
				.OnObjectChanged(this, &SStaticMeshArrayWidget::OnMeshSelected, Item)
			]
		];
}

void SStaticMeshArrayWidget::OnMeshSelected(const FAssetData& AssetData, TSharedPtr<FMeshEntry> Item)
{
	if (Item.IsValid())
	{
		Item->Mesh = Cast<UStaticMesh>(AssetData.GetAsset());
		ListViewWidget->RequestListRefresh(); 
	}
}
