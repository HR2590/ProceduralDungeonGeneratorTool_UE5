#pragma once
#include "DungeonData.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"



class SComboBoxWithButton : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SComboBoxWithButton) {}
        SLATE_ARGUMENT(TSharedPtr<FDungeonData>,RoomData)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<FDungeonData> RoomData;
    
    /** Opciones del ComboBox */
    TArray<TSharedPtr<FString>> Options;
    
    /** Opción actualmente seleccionada */
    TSharedPtr<FString> SelectedOption;
    TArray<FString> TextOptions={
        TEXT("BSP Random Algorithm"),
        TEXT("BSP Uniform Algorithm")};
    

    /** Genera cada elemento del ComboBox */
    TSharedRef<SWidget> GenerateComboItem(TSharedPtr<FString> Item);

    /** Maneja el cambio de opción en el ComboBox */
    void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);

    /** Retorna el texto de la opción seleccionada */
    FText GetCurrentOptionText() const;

    /** Acción cuando se presiona el botón "Use" */
    FReply OnUseButtonClicked();
    
    

    

};
