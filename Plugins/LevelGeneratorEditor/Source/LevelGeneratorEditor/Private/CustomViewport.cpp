// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomViewport.h"


/*
bool FCustomViewport::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (EventArgs.Key == EKeys::LeftMouseButton && EventArgs.Event == IE_Pressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clic detectado en el viewport del editor"));

		// Obtener la posición del mouse en pantalla
		FIntPoint MousePosition;
		Viewport->GetMousePos(MousePosition);

		// Obtener la vista de la escena (necesaria para deproyectar)
		FSceneViewFamilyContext ViewFamily(
			FSceneViewFamily::ConstructionValues(Viewport, GetWorld()->Scene, FEngineShowFlags(ESFIM_Editor))
		);

		FSceneView* SceneView = CalcSceneView(&ViewFamily);
		if (!SceneView)
		{
			UE_LOG(LogTemp, Error, TEXT("No se pudo obtener la vista del editor."));
			return false;
		}

		// Convertir la posición del mouse en coordenadas de mundo
		FVector WorldLocation, WorldDirection;
		SceneView->DeprojectFVector2D(MousePosition, WorldLocation, WorldDirection);

		// Trazar un rayo en el mundo del editor
		FHitResult HitResult;
		FVector End = WorldLocation + (WorldDirection * 10000.0f);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_Visibility))
		{
			UE_LOG(LogTemp, Warning, TEXT("Objeto clickeado: %s"), *HitResult.GetActor()->GetName());
		}

		return true; // Indicar que se manejó el input
	}

	return FEditorViewportClient::InputKey(EventArgs);
}*/




void FCustomViewport::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX,
                                   uint32 HitY)
{
	if (Key == EKeys::LeftMouseButton && Event == IE_Pressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clic detectado en el viewport del editor"));
	}
	
	FEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);
}


