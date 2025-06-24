// Copyright Epic Games, Inc. All Rights Reserved.


#include "LevelGeneratorEditor.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelGeneratorEditorCommands.h"
#include "LevelGeneratorEditorStyle.h"
#include "ToolMenus.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Kismet2/KismetDebugUtilities.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"


static const FName LevelGeneratorEditorTabName("LevelGeneratorEditor");

#define LOCTEXT_NAMESPACE "FLevelGeneratorEditorModule"

void FLevelGeneratorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLevelGeneratorEditorStyle::Initialize();
	FLevelGeneratorEditorStyle::ReloadTextures();

	
	FLevelGeneratorEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLevelGeneratorEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FLevelGeneratorEditorModule::OpenSlateMenu),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLevelGeneratorEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LevelGeneratorEditorTabName,
	   FOnSpawnTab::CreateRaw(this, &FLevelGeneratorEditorModule::OnSpawnTab))
	   .SetDisplayName(LOCTEXT("FLevelGen Tab", "Level Generator Editor"))
	   .SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
	   .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.WorldProperties")) // 🔹 Icono similar a World Settings
	   .SetAutoGenerateMenuEntry(false); //
	

//	FEditorDelegates::OnNewActorsDropped.AddRaw(this,&FLevelGeneratorEditorModule::OnActorsDragged);
//	FEditorDelegates::OnEditorInitialized.AddRaw(this,&FLevelGeneratorEditorModule::OnEditorInitialized);


}

void FLevelGeneratorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FLevelGeneratorEditorStyle::Shutdown();

	FLevelGeneratorEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LevelGeneratorEditorTabName);
}

TSharedRef<SDockTab> FLevelGeneratorEditorModule::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
	
	const FString Path = TEXT("/Game/Blueprints/Assets/Widgets/BP_DungeonGenerator"); // Ajusta esto al path real

	if (auto WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *Path))
	{
		FKismetDebugUtilities::ClearBreakpoints(WidgetBP);

		auto Widget =CreateWidget<UUserWidget>(GEditor->GetEditorWorldContext().World(),
			Cast<UClass>(WidgetBP->GeneratedClass));
		return SNew(SDockTab)
			.TabRole(NomadTab)
			[
				Widget->TakeWidget()
			];
	}


	return SNew(SDockTab).TabRole(NomadTab)
		[ SNew(STextBlock).Text(LOCTEXT("ErrorLoading", "Failed to load widget.")) ];

	
	/*
	 return SNew(SDockTab)
        .TabRole(NomadTab)
        .Label(LOCTEXT(Level Gen Editor, Level Generator Editor))
        .OnTabActivated_Lambda([](const TSharedRef<SDockTab>& Tab, ETabActivationCause Cause)
        {
            // Mueve la ventana cerca del World Settings
            Tab->RequestCloseTab();
            FGlobalTabmanager::Get()->TryInvokeTab(LevelGeneratorEditorTabName);
        })
        [
            SNew(SBox)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                	.Text(FText::FromString("External Algorithms"))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
                ]
	            
                + SVerticalBox::Slot()
            	.AutoHeight()
                [
                   SNew(SComboBoxWithButton)
	                .RoomData(RoomData)
                ]
            	+ SVerticalBox::Slot()
					.AutoHeight()
            	[
					 SNew(SCheckBoxCustom)
            	]
            	+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Delete All")))
					.OnClicked(FOnClicked::CreateRaw(this,&FLevelGeneratorEditorModule::OnDeleteButtonClicked)) // Vincula la función de callback
				
				]
            	+ SVerticalBox::Slot()
            		.AutoHeight()
            		[
            			SNew(SStaticMeshArrayWidget)
            			.MeshEntries(RoomData->MeshEntries)
            		]
	            + SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("Width")))
											.InitialValue(RoomData->width)
										]
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("Height")))
											.InitialValue(RoomData->height)
										]
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("MinRoomSizeX")))
											.InitialValue(RoomData->UpdateMinRoomSizeX())
										]
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("MinRoomSizeY")))
											.InitialValue(RoomData->UpdateMinRoomSizeY())
										]
										
									]
            	+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
											.AutoWidth()
										 [
											 SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("ToleranceX")))
											.InitialValue(RoomData->minToleranceX)
										 ]
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											 SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("ToleranceY")))
											.InitialValue(RoomData->minToleranceY)
										 ]
										+ SHorizontalBox::Slot()
											 .AutoWidth()
										[
											 SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("DivisorX")))
											.InitialValue(RoomData->DivisorX)
										]
										+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("DivisorY")))
											.InitialValue(RoomData->DivisorY)
										]
						+ SHorizontalBox::Slot()
											.AutoWidth()
										[
											SNew(STextWithInputValue)
											.InitialLabelText(FText::FromString(TEXT("Depth")))
											.InitialValue(RoomData->Depth)
										]
					]
         
	            // INTERNAL 
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                [
                    SNew(SScrollBox)
                    + SScrollBox::Slot()
                    [
                    	SNew(STextBlock)
						.Text(FText::FromString("Internal Algorithms"))
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
                    ]
                ]
            	+ SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									SNew(SScrollBox)
									+ SScrollBox::Slot()
									[
										SNew(STextWithButton)
										.LabelText(FText::FromString("Genetic Algorithm"))
									]
								]
            	+ SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									SNew(SScrollBox)
									+ SScrollBox::Slot()
									[
										SNew(STextWithButton)
										.LabelText(FText::FromString("ChestBox Generator"))
									]
								]
            	+ SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									SNew(SScrollBox)
									+ SScrollBox::Slot()
									[
										SNew(STextWithButton)
										.LabelText(FText::FromString("Enemy Spawner"))
									]
								]
            	+ SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									SNew(SScrollBox)
									+ SScrollBox::Slot()
									[
										SNew(SDynamicMeshArray)
									]
								]
	            //BRUSH
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                [
                    SNew(SScrollBox)
                    + SScrollBox::Slot()
                    [
                    	SNew(STextBlock)
						.Text(FText::FromString("Automatic Brush"))
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
                    ]
                ]
            ]
        ];*/

	
}




void FLevelGeneratorEditorModule::OpenSlateMenu()
{
	
	TSharedPtr<FTabManager> LevelEditorTabManager = FGlobalTabmanager::Get();
	
	TSharedPtr<SDockTab> LevelGeneratorTab = LevelEditorTabManager->FindExistingLiveTab(LevelGeneratorEditorTabName);

	if (!LevelGeneratorTab.IsValid()) LevelEditorTabManager->TryInvokeTab(LevelGeneratorEditorTabName);
	
}

void FLevelGeneratorEditorModule::OnEditorInitialized(double InDuration)
{
//	RoomData=MakeShared<FRoomData>();
//	GEditor->GetEditorSubsystem<UBSPDungeonGenerator>()->AssignData(RoomData);
	
}


void FLevelGeneratorEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Level Generator Editor");
			Section.AddMenuEntryWithCommandList(FLevelGeneratorEditorCommands::Get().PluginAction, PluginCommands);
		}
	}
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.WorldSettings");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Level Generator Editor");
			Section.AddMenuEntryWithCommandList(FLevelGeneratorEditorCommands::Get().PluginAction, PluginCommands);
		}
	}
	
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLevelGeneratorEditorModule, LevelGeneratorEditor)