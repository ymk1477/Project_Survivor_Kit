// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"
#include "Weapon_Manager.h"

// Sets default values
AWeapon_Manager::AWeapon_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon_Manager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

