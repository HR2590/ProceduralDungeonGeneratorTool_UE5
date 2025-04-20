#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"


class STextWithButton : public SCompoundWidget
{
public:
    /** Define un delegado para manejar clics personalizados */
    DECLARE_DELEGATE(FOnButtonClicked)

    SLATE_BEGIN_ARGS(STextWithButton) 
        : _LabelText(FText::FromString("Default Label")) // Valor por defecto
    {}
        SLATE_ARGUMENT(FText, LabelText) // Permite cambiar el texto al crear
        SLATE_EVENT(FOnButtonClicked, OnButtonClicked) // Permite una función personalizada en cada instancia
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        LabelText = InArgs._LabelText; // Guarda el texto personalizado
        OnButtonClicked = InArgs._OnButtonClicked; // Guarda la función personalizada

        ChildSlot
        [
            SNew(SHorizontalBox)

            // 🔹 Texto alineado a la izquierda
            + SHorizontalBox::Slot()
            //.FillWidth(1.0f)
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(5)
            [
                SNew(STextBlock)
                .Text(LabelText)
                //.Justification(ETextJustify::Left)
            ]

            // 🔹 Botón alineado a la derecha
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(5)
            [
                SNew(SButton)
                .Text(FText::FromString("Use"))
                .OnClicked(this, &STextWithButton::HandleButtonClick) // Llama a la función personalizada
            ]
        ];
    }

private:
    /** Texto del label */
    FText LabelText;

    /** Delegado para la función personalizada del botón */
    FOnButtonClicked OnButtonClicked;

    /** Maneja el clic del botón y ejecuta la función personalizada */
    FReply HandleButtonClick()
    {
        if (OnButtonClicked.IsBound())
        {
            OnButtonClicked.Execute();
        }
        return FReply::Handled();
    }
};

