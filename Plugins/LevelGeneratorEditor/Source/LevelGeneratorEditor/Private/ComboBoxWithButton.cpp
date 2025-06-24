
#include "ComboBoxWithButton.h"

#include "BSPDungeonGenerator.h"


void SComboBoxWithButton::Construct(const FArguments& InArgs)
{
	//Initialize values
	RoomData=InArgs._RoomData;
	
	for (const auto InOption : TextOptions)
		Options.Add(MakeShareable(new FString(InOption)));
	
	SelectedOption = Options[0];


	//Child slot
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			// 🔹 ComboBox
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&Options)
				.OnGenerateWidget(this, &SComboBoxWithButton::GenerateComboItem)
				.OnSelectionChanged(this, &SComboBoxWithButton::OnSelectionChanged)
				.InitiallySelectedItem(SelectedOption)
				[
					SNew(STextBlock)
					.Text(this, &SComboBoxWithButton::GetCurrentOptionText)
				]
			]

			// 🔹 Botón "Use"
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5, 0, 0, 0)
			[
				SNew(SButton)
				.Text(FText::FromString("Use"))
				.OnClicked(this, &SComboBoxWithButton::OnUseButtonClicked)
			]
		]
	];
}

TSharedRef<SWidget> SComboBoxWithButton::GenerateComboItem(TSharedPtr<FString> Item)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*Item));
}

void SComboBoxWithButton::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	SelectedOption = NewValue;
}

FText SComboBoxWithButton::GetCurrentOptionText() const
{
	return SelectedOption.IsValid() ? FText::FromString(*SelectedOption) : FText::FromString("Select an Option");
}

FReply SComboBoxWithButton::OnUseButtonClicked()
{
	//*RoomData->minBaseRoomSizeX=*RoomData->minMeshSizeX**RoomData->minRoomSizeX;
	//*RoomData->minBaseRoomSizeY=*RoomData->minMeshSizeY**RoomData->minRoomSizeY;
	/*
	if (SelectedOption.IsValid() && SelectedOption->Equals(TextOptions[1]))
	{
		GEditor->GetEditorSubsystem<UBSPDungeonGenerator>()->BSPUniformAlgorithm();
	}
	else if(SelectedOption.IsValid() && SelectedOption->Equals(TextOptions[0]))
	{
		GEditor->GetEditorSubsystem<UBSPDungeonGenerator>()->BSPRandomAlgorithm();
	}
	*/
	return FReply::Handled();
	
}

