#pragma once

class STextWithInputValue: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STextWithInputValue) {}
		SLATE_ARGUMENT(FText, InitialLabelText) // Atributo para el texto fijo
		SLATE_ARGUMENT(TSharedPtr<uint32>, InitialValue) // Atributo para el texto fijo
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	FText LabelText;
private:
	TSharedPtr<uint32> EditableValue =nullptr;
	void OnValueChanged(const FText& InValue);
	void OnValueCommited(const FText& InValue,ETextCommit::Type InCommitType);
	FText GetValueAsText() const
	{
		if (!EditableValue)return FText::FromString("0");
		return FText::AsNumber(*EditableValue);
	}

};
