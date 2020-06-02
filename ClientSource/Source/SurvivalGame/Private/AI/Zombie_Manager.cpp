// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"

#include "Zombie_Manager.h"

// Sets default values
AZombie_Manager::AZombie_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AZombie_Manager::FindSpawnPoints()
{
	UWorld* CurrentWorld = GetWorld();
	for (TActorIterator<AZombieSpawnPoint> It(CurrentWorld); It; ++It)
	{
		AZombieSpawnPoint* NewPoint = *It;
		ZombieSpawnPoints.Emplace(NewPoint);
	}
}
void AZombie_Manager::SpawnZombies()
{
	UWorld* CurrentWorld = GetWorld();
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/zombie/ZombieCharacter")));
	UBlueprint* GenerateBp = Cast<UBlueprint>(SpawnActor);

	FActorSpawnParameters Spawnparams;
	Spawnparams.Owner = this;
	Spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (auto i = ZombieSpawnPoints.begin(); i != ZombieSpawnPoints.end(); ++i)
	{
		ASZombieCharacter* NewZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewZombie->GetController());
		ZombieController->Possess(NewZombie);
	}
}

// Called when the game starts or when spawned
void AZombie_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	FindSpawnPoints();
	SpawnZombies();
}

// Called every frame
void AZombie_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

