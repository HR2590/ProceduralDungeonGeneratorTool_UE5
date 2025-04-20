#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SCheckBox.h"


class SCheckBoxCustom : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCheckBoxCustom) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		ChildSlot
		[
		   SNew(SVerticalBox)
		   + SVerticalBox::Slot()
		   .AutoHeight()
		   .Padding(5)
		   [
			  SNew(SCheckBox)
			  .OnCheckStateChanged(this, &SCheckBoxCustom::OnCheckBoxChanged)
			  .IsChecked(ECheckBoxState::Unchecked)
			  .Content()
			  [
				 SNew(STextBlock)
				 .Text(FText::FromString("Test"))
			  ]
		   ]
		];
	}

private:
	void OnCheckBoxChanged(ECheckBoxState NewState)
	{
		if (NewState == ECheckBoxState::Checked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Checkbox checked!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Checkbox unchecked!"));
		}
	}
};
