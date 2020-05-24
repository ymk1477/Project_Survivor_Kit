// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"
#include "Player_Manager.h"



// Sets default values
APlayer_Manager::APlayer_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer_Manager::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player_Manager Begin Play!! ")));

	SetActorTickInterval(0.1f);
	

	MakeStartLocation();
	SpawnPlayers();
	
}

// Called every frame
void APlayer_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Connected : %d"), PlayerId));
	//bool s = ASCoopGameMode::IsStart();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PlayerManager Tick()")));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Connected : %d "),Connected));
	if (Connected)
	{
		S_Players S_Packet;
		S_Packet.Loc = Player_info.Loc[PlayerId];
		S_Packet.IsJump = Player_info.IsJump[PlayerId];
		MySocket::sendBuffer(PACKET_CS_PLAYERS, &S_Packet);
		MySocket::RecvPacket();
		
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (i != PlayerId) 
			{
				if (Player_info.IsUsed[i])
				{
					FVector NewLocation;
					NewLocation.X = Player_info.Loc[i].x;
					NewLocation.Y = Player_info.Loc[i].y;
					NewLocation.Z = Player_info.Loc[i].z;
					players[i]->SetActorLocation(NewLocation);
				}
			}
		}
	}


	if (!test)
	{
		
		test = true;
	}
}


void APlayer_Manager::MakeStartLocation()
{
	UWorld* World = GetWorld();
	ASCharacter* MyCharacter = Cast<ASCharacter>(World->GetFirstPlayerController()->GetCharacter());
	players.Emplace(MyCharacter);

	FVector PlayerStart = MyCharacter->GetActorLocation();		// 플레이어별 시작지점 지정
	StartLocation.Emplace(PlayerStart);							// Player1
	PlayerStart.Y += 200;
	StartLocation.Emplace(PlayerStart);							// Player2
	PlayerStart.X -= 200;
	StartLocation.Emplace(PlayerStart);							// Player3
	PlayerStart.Y -= 200;
	StartLocation.Emplace(PlayerStart);							// Player4
}

void APlayer_Manager::SpawnPlayers()
{
	UWorld* World = GetWorld();
	if (Playing > 1)
	{
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Player/PlayerPawn")));

		UBlueprint* GenerateBp = Cast<UBlueprint>(SpawnActor);

		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CAN'T FIND OBJECT TO SPAWN")));
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		}


		FActorSpawnParameters Spawnparams;
		Spawnparams.Owner = this;
		Spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		APlayerController* Mycontroller = World->GetFirstPlayerController();
		for (int i = 1; i < Playing; ++i) {
			ASCharacter* NewCharacter = World->SpawnActor<ASCharacter>(GenerateBp->GeneratedClass, StartLocation[i], FRotator::ZeroRotator, Spawnparams);

			players.Emplace(NewCharacter);
		}
		if (PlayerId != 0)
			Mycontroller->Possess(players[PlayerId]);
	}
}