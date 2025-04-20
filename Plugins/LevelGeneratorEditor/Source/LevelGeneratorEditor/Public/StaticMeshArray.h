#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Views/SListView.h"
#include "MeshEntry.h"



class SStaticMeshArrayWidget : public SCompoundWidget
{
   
public:
    SLATE_BEGIN_ARGS(SStaticMeshArrayWidget){}
        SLATE_ARGUMENT(TArray<TSharedPtr<FMeshEntry>>, MeshEntries) 
    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs);


private:
    
    /** Lista de elementos */
    TArray<TSharedPtr<FMeshEntry>> StaticMeshEntries;

    /** Widget de la lista */
    TSharedPtr<SListView<TSharedPtr<FMeshEntry>>> ListViewWidget;

    /** Genera una fila en la lista */
    TSharedRef<ITableRow> GenerateMeshRow(TSharedPtr<FMeshEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);

    /** Maneja la selección de un nuevo Static Mesh */
    void OnMeshSelected(const FAssetData& AssetData, TSharedPtr<FMeshEntry> Item);
};
