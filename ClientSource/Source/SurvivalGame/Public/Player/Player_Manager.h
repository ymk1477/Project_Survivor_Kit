// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SurvivalGame.h"
#include "CoreMinimal.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "SCoopGameMode.h"
#include "MyGameInstance.h"
#include "SPlayerController.h"
#include "SPlayerCameraManager.h"
#include "SLocalPlayer.h"
#include "MyGameInstance.h"
#include "SCharacter.h"
#include "AIController.h"
#include "SZombieCharacter.h"
#include "Player_Manager.generated.h"


UCLASS()
class SURVIVALGAME_API APlayer_Manager : public AActor
{
	GENERATED_BODY()

		//ASCharacter* Array_Player[MAX_USER] = { NULL };

	TArray<ASCharacter*> players;
	TArray<FVector> StartLocation;
	bool test = false;

public:
	// Sets default values for this actor's properties
	APlayer_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MakeStartLocation();
	void SpawnPlayers();
	//void CheckNewPlayer();
};

