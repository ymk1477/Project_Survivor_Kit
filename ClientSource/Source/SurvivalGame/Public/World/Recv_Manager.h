// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"

#include "Recv_Manager.generated.h"


UCLASS()
class SURVIVALGAME_API ARecv_Manager : public AActor
{
	GENERATED_BODY()

	APlayerController* controller;
public:	
	// Sets default values for this actor's properties
	ARecv_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
