// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"
#include "MyGameInstance.h"

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

	for (TActorIterator< APatrolZombieSpawnPoint> It(CurrentWorld); It; ++It)
	{
		APatrolZombieSpawnPoint* NewPoint = *It;
		PatrolZombieSpawnPoints.Emplace(NewPoint);
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

	FRotator NewRotator;

	int ZombieNum = 0;

	for (auto i = ZombieSpawnPoints.begin(); i != ZombieSpawnPoints.end(); ++i)
	{
		//NewRotator.Yaw = FMath::RandRange(0.0f, 360.0f);
		ASZombieCharacter* NewZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewZombie->GetController());
		Zombies.Emplace(NewZombie);
		ZombieController->Possess(NewZombie);
		Zombie_info.HP[ZombieNum] = NewZombie->GetHealth();
		Zombie_info.IsAlive[ZombieNum] = true;
		ZombieNum++;
	}		

	UObject* SpawnActor2 = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/zombie/ZombiePatrol1")));
	UBlueprint* GenerateBp2 = Cast<UBlueprint>(SpawnActor2);

	for (auto i = PatrolZombieSpawnPoints.begin(); i != PatrolZombieSpawnPoints.end(); ++i)
	{
		//NewRotator.Yaw = FMath::RandRange(0.0f, 360.0f);
		ASZombieCharacter* NewPatrolZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp2->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		//ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewPatrolZombie->GetController());
		PatrolZombies.Emplace(NewPatrolZombie);
		//ZombieController->Possess(NewPatrolZombie);
	}

}

void AZombie_Manager::FindWayPoints()
{
	UWorld* CurrentWorld = GetWorld();
	for (TActorIterator<ASBotWaypoint> It(CurrentWorld); It; ++It)
	{
		ASBotWaypoint* NewPoint = *It;
		Waypoints.Emplace(NewPoint);
	}
}

// Called when the game starts or when spawned
void AZombie_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	FindSpawnPoints();
	FindWayPoints();
	SpawnZombies();
}

// Called every frame
void AZombie_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	int32 indexNum = -1;
	if (Zombies.Num() > 0)
	{
		for (int32 i = 0; i < Zombies.Num(); ++i)
		{
			if (Zombies[i]->IsDie())
			{
				indexNum = i;
				break;
			}
		}
		if (Zombies.IsValidIndex(indexNum))
		{
			Zombies.RemoveAt(indexNum);
		}
	}

	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Zobies : %d "),
		Zombies.Num()));*/

	indexNum = -1;
	if (PatrolZombies.Num() > 0)
	{
		for (int32 i = 0; i < PatrolZombies.Num(); ++i)
		{
			if (PatrolZombies[i]->IsDie())
			{
				indexNum = i;
				break;
			}
		}
		if (PatrolZombies.IsValidIndex(indexNum))
		{
			PatrolZombies.RemoveAt(indexNum);
		}
	}
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ZombieSpawnPoints : %d , PatrolZombiesSpawnPoints : %d "),
		ZombieSpawnPoints.Num(), PatrolZombieSpawnPoints.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Zombies : %d , PatrolZombies : %d "),
		Zombies.Num(),PatrolZombies.Num()));*/

}

TArray<ASZombieCharacter*>* AZombie_Manager::GetZombieArray()
{
	return &Zombies;
}

TArray<ASBotWaypoint*> AZombie_Manager::GetWayPoints()
{
	return Waypoints;
}

TArray<ASZombieCharacter*> AZombie_Manager::GetPatrolZombies()
{
	return PatrolZombies;
}