// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGRoom.h"


APCGRoomBase::APCGRoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APCGRoomBase::BeginPlay()
{
	Super::BeginPlay();
}

void APCGRoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APCGRoom::APCGRoom()
{
	PrimaryActorTick.bCanEverTick = false;
}





