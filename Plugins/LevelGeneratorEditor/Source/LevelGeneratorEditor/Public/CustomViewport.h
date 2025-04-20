// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */




class LEVELGENERATOREDITOR_API FCustomViewport : public FEditorViewportClient
{
public:

	FCustomViewport(FPreviewScene* InPreviewScene)
	   : FEditorViewportClient(nullptr, InPreviewScene)
	{
	}
	//virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;

	virtual void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;

};
