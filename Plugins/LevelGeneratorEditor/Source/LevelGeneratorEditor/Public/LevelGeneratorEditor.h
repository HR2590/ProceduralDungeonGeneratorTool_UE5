// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RoomData.h"
#include "Modules/ModuleManager.h"

class UBSPDungeonGenerator;
class FToolBarBuilder;
class FMenuBuilder;

DECLARE_DELEGATE(FOnCustomButtonClicked);

class FLevelGeneratorEditorModule : public IModuleInterface
{
protected:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs);
	void OpenSlateMenu();
	void OnEditorInitialized(double InDuration);

private:
	TSharedPtr<FRoomData> RoomData;
	FOnCustomButtonClicked OnCustomButtonClicked;
	void OnActorsDragged(const TArray<UObject*>& Objects,const TArray<AActor*>& Actors);
	void RegisterMenus();
	FReply OnDeleteButtonClicked();
	TSharedPtr<FUICommandList> PluginCommands;


	
	
};
