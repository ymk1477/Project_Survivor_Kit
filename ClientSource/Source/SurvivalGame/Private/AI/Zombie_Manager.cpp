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

	for (TActorIterator< AFastZombieSpawnPoint> It(CurrentWorld); It; ++It)
	{
		AFastZombieSpawnPoint* NewPoint = *It;
		FastZombieSpawnPoints.Emplace(NewPoint);
	}

	for (TActorIterator< ACoronaZombieSpawnPoint> It(CurrentWorld); It; ++It)
	{
		ACoronaZombieSpawnPoint* NewPoint = *It;
		CoronaZombieSpawnPoints.Emplace(NewPoint);
	}

	for (TActorIterator< AZombieAttackPoint> It(CurrentWorld); It; ++It)
	{
		AZombieAttackPoint* NewPoint = *It;
		ZombieAttackPoints.Emplace(NewPoint);
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
		Zombie_info.Target[ZombieNum] = -1;
		Zombie_info.Loc[ZombieNum].x = NewZombie->GetActorLocation().X;
		Zombie_info.Loc[ZombieNum].y = NewZombie->GetActorLocation().Y;
		Zombie_info.Loc[ZombieNum].z = NewZombie->GetActorLocation().Z;
		NewZombie->Zombie_Index = ZombieNum;
		++ZombieNum;
	}

	UObject* SpawnActor2 = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/zombie/ZombiePatrol1.ZombiePatrol1")));
	UBlueprint* GenerateBp2 = Cast<UBlueprint>(SpawnActor2);

	for (auto i = PatrolZombieSpawnPoints.begin(); i != PatrolZombieSpawnPoints.end(); ++i)
	{
		ASZombieCharacter* NewPatrolZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp2->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewPatrolZombie->GetController());
		Zombies.Emplace(NewPatrolZombie);
		ZombieController->Possess(NewPatrolZombie);
		Zombie_info.HP[ZombieNum] = NewPatrolZombie->GetHealth();
		Zombie_info.IsAlive[ZombieNum] = true;
		Zombie_info.Target[ZombieNum] = -1;
		Zombie_info.Loc[ZombieNum].x = NewPatrolZombie->GetActorLocation().X;
		Zombie_info.Loc[ZombieNum].y = NewPatrolZombie->GetActorLocation().Y;
		Zombie_info.Loc[ZombieNum].z = NewPatrolZombie->GetActorLocation().Z;
		NewPatrolZombie->Zombie_Index = ZombieNum;
		++ZombieNum;
	}

	UObject* SpawnActor3 = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/FreeModels/Zombie_3/Zombie_Fast")));
	UBlueprint* GenerateBp3 = Cast<UBlueprint>(SpawnActor3);

	for (auto i = FastZombieSpawnPoints.begin(); i != FastZombieSpawnPoints.end(); ++i)
	{
		ASZombieCharacter* NewFastZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp3->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewFastZombie->GetController());
		Zombies.Emplace(NewFastZombie);
		ZombieController->Possess(NewFastZombie);
		Zombie_info.HP[ZombieNum] = NewFastZombie->GetHealth();
		Zombie_info.IsAlive[ZombieNum] = true;
		Zombie_info.Target[ZombieNum] = -1;
		Zombie_info.Loc[ZombieNum].x = NewFastZombie->GetActorLocation().X;
		Zombie_info.Loc[ZombieNum].y = NewFastZombie->GetActorLocation().Y;
		Zombie_info.Loc[ZombieNum].z = NewFastZombie->GetActorLocation().Z;
		NewFastZombie->Zombie_Index = ZombieNum;
		++ZombieNum;;
	}

	UObject* SpawnActor4 = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/FreeModels/Zombie_4/CoronaZombie.CoronaZombie")));
	UBlueprint* GenerateBp4 = Cast<UBlueprint>(SpawnActor4);

	for (auto i = CoronaZombieSpawnPoints.begin(); i != CoronaZombieSpawnPoints.end(); ++i)
	{
		ASZombieCharacter* NewCoronaZombie = CurrentWorld->SpawnActor<ASZombieCharacter>(GenerateBp4->GeneratedClass, (*i)->GetActorLocation(), FRotator::ZeroRotator, Spawnparams);
		ASZombieAIController* ZombieController = Cast<ASZombieAIController>(NewCoronaZombie->GetController());
		Zombies.Emplace(NewCoronaZombie);
		ZombieController->Possess(NewCoronaZombie);
		Zombie_info.HP[ZombieNum] = NewCoronaZombie->GetHealth();
		Zombie_info.IsAlive[ZombieNum] = true;
		Zombie_info.Target[ZombieNum] = -1;
		Zombie_info.Loc[ZombieNum].x = NewCoronaZombie->GetActorLocation().X;
		Zombie_info.Loc[ZombieNum].y = NewCoronaZombie->GetActorLocation().Y;
		Zombie_info.Loc[ZombieNum].z = NewCoronaZombie->GetActorLocation().Z;
		NewCoronaZombie->Zombie_Index = ZombieNum;
		++ZombieNum;
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

	PatrolZombieNumber = 0;
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
				Zombie_info.IsAlive[i] = false;
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

	/*indexNum = -1;
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
	}*/
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ZombieSpawnPoints : %d , PatrolZombiesSpawnPoints : %d "),
		ZombieSpawnPoints.Num(), PatrolZombieSpawnPoints.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Zombies : %d , PatrolZombies : %d "),
		Zombies.Num(),PatrolZombies.Num()));*/



		/*for (int32 i = 0; i < Zombies.Num(); ++i)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d Zombie HP : %d "),
				i, Zombies[i]->GetHealth()));
		}*/
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

int32 AZombie_Manager::GetPatrolNumber()
{
	PatrolZombieNumber++;
	return PatrolZombieNumber;
}

void AZombie_Manager::ZombieAttackSpawn()
{
	for (int i = 0; i < Zombies.Num(); ++i)
	{
		Zombies[i]->Destroy();
	}
	Zombies.Empty();
	SpawnZombies();
	/*auto AttackPoints = ZombieAttackPoints.begin();
	for (auto p = Zombies.begin(); p != Zombies.end(); ++p)
	{
		(*p)->SetActorLocation((*AttackPoints)->GetActorLocation(), true, nullptr, ETeleportType::None);
		AttackPoints++;
	}*/

	for (int i = 0; i < Zombies.Num(); ++i)
	{
		Zombies[i]->SetActorLocation(ZombieAttackPoints[i]->GetActorLocation());
	}
}