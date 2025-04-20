// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelGeneratorEditorCommands.h"

#define LOCTEXT_NAMESPACE "FLevelGeneratorEditorModule"

void FLevelGeneratorEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "LevelGeneratorEditor", "Execute LevelGeneratorEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
