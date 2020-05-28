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

	SetActorTickInterval(0.016f);


	MakeStartLocation();
	SpawnPlayers();

	/*S_LevelChange s_packet;
	s_packet.changed = true;
	MySocket::sendBuffer(PACKET_CS_LEVEL_CHANGE, &s_packet);*/
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PlayerId : %d "), PlayerId));
		
		for (int i = 0; i < MAX_USER; ++i)
		{
			if(Player_info.IsUsed[i])
				if (players[i]->IsControlled())
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d Player Controller: TRUE "), i));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d Player Controller: FALSE "), i));
				}
		}

		ASPlayerController* Mycontroller = Cast<ASPlayerController>(GetWorld()->GetFirstPlayerController());
		ASCharacter* MyPawn = Cast<ASCharacter>(Mycontroller->GetPawn());

		FVector MyLocation = MyPawn->GetActorLocation();
		Player_info.Loc[PlayerId].x = MyLocation.X;
		Player_info.Loc[PlayerId].y = MyLocation.Y;
		Player_info.Loc[PlayerId].z = MyLocation.Z;

		FRotator MyRotation = MyPawn->GetActorRotation();
		Player_info.Rot[PlayerId].yaw = MyRotation.Yaw;
		Player_info.Rot[PlayerId].pitch = MyRotation.Pitch;
		Player_info.Rot[PlayerId].roll = MyRotation.Roll;

		FVector MyVelocity = MyPawn->GetVelocity();
		Player_info.Vel[PlayerId].x = MyVelocity.X;
		Player_info.Vel[PlayerId].y = MyVelocity.Y;
		Player_info.Vel[PlayerId].z = MyVelocity.Z;

		FRotator MyAim = MyPawn->GetAimOffsets();
		Player_info.Aim[PlayerId].yaw = MyAim.Yaw;
		Player_info.Aim[PlayerId].pitch = MyAim.Pitch;
		Player_info.Aim[PlayerId].roll = MyAim.Roll;

		S_Players S_Packet;
		S_Packet.Loc = Player_info.Loc[PlayerId];
		S_Packet.Rot = Player_info.Rot[PlayerId];
		S_Packet.Vel = Player_info.Vel[PlayerId];
		S_Packet.Aim = Player_info.Aim[PlayerId];
		S_Packet.IsJump = Player_info.IsJump[PlayerId];
		S_Packet.IsTargeting = Player_info.IsTargeting[PlayerId];
		S_Packet.IsSprinting = Player_info.IsSprinting[PlayerId];
		S_Packet.onCrouchToggle = Player_info.onCrouchToggle[PlayerId];
		MySocket::sendBuffer(PACKET_CS_PLAYERS, &S_Packet);
		Player_info.onCrouchToggle[PlayerId] = false;
		MySocket::RecvPacket();

		for (int i = 0; i < MAX_USER; ++i)
		{
			if ( (i != PlayerId) && Player_info.IsUsed[i] )
			{

				FVector NewLocation;
				NewLocation.X = Player_info.Loc[i].x;
				NewLocation.Y = Player_info.Loc[i].y;
				NewLocation.Z = Player_info.Loc[i].z;
				FRotator NewRotation;
				NewRotation.Pitch = Player_info.Rot[i].pitch;
				NewRotation.Yaw = Player_info.Rot[i].yaw;
				NewRotation.Roll = Player_info.Rot[i].roll;
				FVector NewVelocity;
				NewVelocity.X = Player_info.Vel[i].x;
				NewVelocity.Y = Player_info.Vel[i].y;
				NewVelocity.Z = Player_info.Vel[i].z;
				FRotator NewAim;
				NewAim.Pitch = Player_info.Aim[i].pitch;
				NewAim.Yaw = Player_info.Aim[i].yaw;
				NewAim.Roll = Player_info.Aim[i].roll;

				players[i]->SetActorLocationAndRotation(NewLocation, NewRotation, false, 0, ETeleportType::TeleportPhysics);
				players[i]->GetMovementComponent()->Velocity = NewVelocity;
				players[i]->SetAimOffset(NewAim);
				players[i]->SetIsJumping(Player_info.IsJump[i]);
				players[i]->SetIsTargeting(Player_info.IsTargeting[i]);
				players[i]->SetSprinting(Player_info.IsSprinting[i]);
				if (Player_info.onCrouchToggle[i])
					players[i]->OnCrouchToggle();


				/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player Sprinting -> %d"),
					i + 1, players[i]->IsSprinting()));*/
					/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player Com VELOCITY -> x : %f, y : %f, z : %f"),
						i + 1, players[i]->GetRootComponent()->ComponentVelocity.X, players[i]->GetRootComponent()->ComponentVelocity.Y, players[i]->GetRootComponent()->ComponentVelocity.Z));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player Get VELOCITY -> x : %f, y : %f, z : %f"),
						i + 1, players[i]->GetVelocity().X, players[i]->GetVelocity().Y, players[i]->GetVelocity().Z));*/


			}
		}
			

		//MySocket::RecvPacket();

	}

}


void APlayer_Manager::MakeStartLocation()
{
	UWorld* World = GetWorld();
	ASCharacter* MyCharacter = Cast<ASCharacter>(World->GetFirstPlayerController()->GetCharacter());
	players.Emplace(MyCharacter);

	FVector PlayerStart = MyCharacter->GetActorLocation();		// 플레이어별 시작지점 지정
	StartLocation.Emplace(PlayerStart);							// Player1
	PlayerStart.Y += 300;
	StartLocation.Emplace(PlayerStart);							// Player2
	PlayerStart.X -= 300;
	StartLocation.Emplace(PlayerStart);							// Player3
	PlayerStart.Y -= 300;
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

		ASPlayerController* Mycontroller = Cast<ASPlayerController>(World->GetFirstPlayerController());
		for (int i = 1; i < Playing; ++i) {
			ASCharacter* NewCharacter = World->SpawnActor<ASCharacter>(GenerateBp->GeneratedClass, StartLocation[i], FRotator::ZeroRotator, Spawnparams);

			players.Emplace(NewCharacter);
		}
		if (PlayerId != 0)
		{
			Mycontroller->Possess(players[PlayerId]);
		}
	}

	
}
