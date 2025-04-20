
#include "TextWithInputValue.h"

void STextWithInputValue::Construct(const FArguments& InArgs)
{
	LabelText = InArgs._InitialLabelText;
	EditableValue=InArgs._InitialValue;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(LabelText) 
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5)
		[
			SNew(SEditableTextBox)
			                      .Text(GetValueAsText()) // Valor inicial editable
								  .OnTextChanged(this, &STextWithInputValue::OnValueChanged)
							      .OnTextCommitted(this, &STextWithInputValue::OnValueCommited)
		]
	];
}

void STextWithInputValue::OnValueChanged(const FText& InValue)
{
	*EditableValue = FCString::Atoi(*InValue.ToString());
}

void STextWithInputValue::OnValueCommited(const FText& InValue,ETextCommit::Type InCommitType)
{
	*EditableValue= FCString::Atoi(*InValue.ToString());
}
