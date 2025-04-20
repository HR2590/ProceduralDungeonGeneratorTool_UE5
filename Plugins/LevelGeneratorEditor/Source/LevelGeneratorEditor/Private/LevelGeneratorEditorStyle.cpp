// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelGeneratorEditorStyle.h"
#include "LevelGeneratorEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FLevelGeneratorEditorStyle::StyleInstance = nullptr;

void FLevelGeneratorEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FLevelGeneratorEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FLevelGeneratorEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("LevelGeneratorEditorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FLevelGeneratorEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("LevelGeneratorEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("LevelGeneratorEditor")->GetBaseDir() / TEXT("Resources"));

	Style->Set("LevelGeneratorEditor.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FLevelGeneratorEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FLevelGeneratorEditorStyle::Get()
{
	return *StyleInstance;
}
