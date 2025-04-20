// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "LevelGeneratorEditorStyle.h"

class FLevelGeneratorEditorCommands : public TCommands<FLevelGeneratorEditorCommands>
{
public:

	FLevelGeneratorEditorCommands()
		: TCommands<FLevelGeneratorEditorCommands>(TEXT("LevelGeneratorEditor"), NSLOCTEXT("Contexts", "LevelGeneratorEditor", "LevelGeneratorEditor Plugin"), NAME_None, FLevelGeneratorEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
