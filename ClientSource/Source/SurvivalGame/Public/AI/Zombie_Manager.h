// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnPoint.h"
#include "PatrolZombieSpawnPoint.h"
#include "FastZombieSpawnPoint.h"
#include "CoronaZombieSpawnPoint.h"
#include "ZombieAttackPoint.h"
#include "SZombieCharacter.h"
#include "SZombieAIController.h"
#include "SBotWayPoint.h"

#include "Zombie_Manager.generated.h"

UCLASS()
class SURVIVALGAME_API AZombie_Manager : public AActor
{
	GENERATED_BODY()

	TArray< AZombieSpawnPoint*> ZombieSpawnPoints;
	TArray< ASZombieCharacter*> Zombies;
	TArray< APatrolZombieSpawnPoint*> PatrolZombieSpawnPoints;
	TArray< ASZombieCharacter*> PatrolZombies;
	TArray< AFastZombieSpawnPoint*> FastZombieSpawnPoints;
	TArray< ACoronaZombieSpawnPoint*> CoronaZombieSpawnPoints;
	TArray< AZombieAttackPoint*> ZombieAttackPoints;

	TArray< ASBotWaypoint*> Waypoints;

	int32 PatrolZombieNumber;
	int AllZombies;
public:	
	// Sets default values for this actor's properties
	AZombie_Manager();

	void FindSpawnPoints();
	void SpawnZombies();
	void FindWayPoints();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<ASZombieCharacter*>* GetZombieArray();

	UFUNCTION(BlueprintCallable, Category = "Patrol Zombie")
	TArray<ASZombieCharacter*> GetPatrolZombies();

	UFUNCTION(BlueprintCallable, Category = "Zombie WayPoint")
	TArray<ASBotWaypoint*> GetWayPoints();

	UFUNCTION(BlueprintCallable, Category = "Patrol Zombie")
	int32 GetPatrolNumber();

	UFUNCTION(BlueprintCallable, Category = "Zombie Attack")
	void ZombieAttackSpawn();
};
