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

	TActorIterator<AZombie_Manager> It(GetWorld());

	zombie_manager = *It;

	/*S_LevelChange s_packet;
	s_packet.changed = true;
	MySocket::sendBuffer(PACKET_CS_LEVEL_CHANGE, &s_packet);*/
}

// Called every frame
void APlayer_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Connected)
	{
		/*if (All_level_Changed)
		{*/
		ASCharacter* MyPawn = players[PlayerId];

		Player_info.HP[PlayerId] = MyPawn->GetHealth();

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

		FVector POVLoc;
		FRotator POVRot;
		if (MyPawn->GetHealth() > 0)
			MyPawn->Controller->GetPlayerViewPoint(POVLoc, POVRot);
		Player_info.View[PlayerId].Loc.x = POVLoc.X;
		Player_info.View[PlayerId].Loc.y = POVLoc.Y;
		Player_info.View[PlayerId].Loc.z = POVLoc.Z;
		Player_info.View[PlayerId].Rot.yaw = POVRot.Yaw;
		Player_info.View[PlayerId].Rot.pitch = POVRot.Pitch;
		Player_info.View[PlayerId].Rot.roll = POVRot.Roll;

		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player View Rot : Pitch - %f , Roll - %f , Yaw - %f"),
			Player_info.View[PlayerId].Rot.pitch, Player_info.View[PlayerId].Rot.roll, Player_info.View[PlayerId].Rot.yaw ));*/

		Player_info.WeaponState[PlayerId] = MyPawn->GetOtherWeaponState();


		S_Players S_Packet;
		S_Packet.HP = Player_info.HP[PlayerId];
		S_Packet.Rot = Player_info.Rot[PlayerId];
		S_Packet.Vel = Player_info.Vel[PlayerId];
		S_Packet.Aim = Player_info.Aim[PlayerId];
		S_Packet.IsJump = Player_info.IsJump[PlayerId];
		S_Packet.IsTargeting = Player_info.IsTargeting[PlayerId];
		S_Packet.IsSprinting = Player_info.IsSprinting[PlayerId];
		S_Packet.onCrouchToggle = Player_info.onCrouchToggle[PlayerId];
		S_Packet.WeaponState = Player_info.WeaponState[PlayerId];
		S_Packet.View = Player_info.View[PlayerId];
		MySocket::sendBuffer(PACKET_CS_PLAYERS, &S_Packet);
		Player_info.onCrouchToggle[PlayerId] = false;

		MySocket::RecvPacket();

		for (int i = 0; i < MAX_USER; ++i)
		{

			if ((i != PlayerId) && Player_info.IsUsed[i])
			{

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

				players[i]->SetOtherHealth(Player_info.HP[i]);

				/*if (players[i]->GetHealth() > Player_info.HP[i])		// 몬스터 동기화 하면서 같이 해줘야함 (TakeDamage).
				{
					FPointDamageEvent DmgEvent;
					DmgEvent.DamageTypeClass = PunchDamageType;
					DmgEvent.Damage = MeleeDamage;

					players[i]->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
				}*/

				/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player View Rot : Pitch - %f , Roll - %f , Yaw - %f"),
					Player_info.View[i].Rot.pitch, Player_info.View[i].Rot.roll, Player_info.View[i].Rot.yaw));*/

				players[i]->SetActorRelativeRotation(NewRotation);
				players[i]->SetAimOffset(NewAim);
				players[i]->GetMovementComponent()->Velocity = NewVelocity;
				players[i]->AddMovementInput(NewVelocity);

				players[i]->SetIsJumping(Player_info.IsJump[i]);
				players[i]->SetIsTargeting(Player_info.IsTargeting[i]);
				players[i]->SetSprinting(Player_info.IsSprinting[i]);
				if (Player_info.onCrouchToggle[i])
					players[i]->OnCrouchToggle();
				if (Player_info.WeaponState[i] == WEAPON_FIRING)
				{
					players[i]->StartFiringOther();
				}
				else if (Player_info.WeaponState[i] == WEAPON_RELOADING)
				{

					players[i]->ReloadingOther();
				}
				else
					players[i]->StopFiringOther();
			}

		}
		//}

		//	// 좀비 샌드리시브
		//auto ZombieArray = zombie_manager->GetZombieArray();
		//for (int i = 0; i < ZombieArray->Num(); ++i)
		//{
		//	if (Zombie_info.HP[i] < (*ZombieArray)[i]->GetHealth())
		//		Zombie_info.Hit[i] = true;
		//}
		//S_Zombies s_zombie_packet;
		//for (int i = 0; i < MAX_ZOMBIE; ++i)
		//{
		//	s_zombie_packet.IsAlive[i] = Zombie_info.IsAlive[i];
		//	s_zombie_packet.HP[i] = Zombie_info.HP[i];
		//	s_zombie_packet.Hit[i] = Zombie_info.Hit[i];
		//}
		//MySocket::sendBuffer(PACKET_CS_ZOMBIE, &S_Packet);
		//for (int i = 0; i < ZombieArray->Num(); ++i)
		//{
		//	Zombie_info.Hit[i] = false;
		//}
		//MySocket::RecvPacket();
		//for (int i = 0; i < MAX_ZOMBIE; ++i)
		//{
		//	if (Zombie_info.IsAlive[i])
		//	{
		//		if (Zombie_info.Hit[i])
		//		{
		//			//FPointDamageEvent PointDmg;

		//			//(*ZombieArray)[i]->OthertakeDamage(26.0f, PointDmg, GetWorld()->GetFirstPlayerController(), this);
		//		}
		//		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d Zombie Hit : %d "),
		//			i, Zombie_info.HP[i]));*/
		//		Zombie_info.Hit[i] = false;
		//	}
		//}
		/*for (int i = 0; i < ZombieArray->Num(); ++i)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d Zombie Hit : %d "),
				i, (*ZombieArray)[i]->GetHealth()));
		}*/

		// 시간 샌드리시브
		ASGameState* MyGameState = Cast<ASGameState>(GetWorld()->GetAuthGameMode()->GameState);
		S_Time Timepacket;
		Timepacket.PlayerNum = PlayerId;
		if (PlayerId == HostPlayer)
		{
			
			Timepacket.ElapsedTime = MyGameState->ElapsedGameMinutes;
		}
		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Elapsed Time : %d"), 
			MyGameState->ElapsedGameMinutes));*/
		MySocket::sendBuffer(PACKET_CS_TIME, &Timepacket);
		MySocket::RecvPacket();
		UMyGameInstance* MyInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
		if(!(MyInstance->IsHost()))
			MyGameState->ElapsedGameMinutes = Elapsed_Time;
		
	}
}



void APlayer_Manager::MakeStartLocation()
{
	UWorld* World = GetWorld();
	ASCharacter* MyCharacter = Cast<ASCharacter>(World->GetFirstPlayerController()->GetCharacter());
	
	players.Emplace(MyCharacter);

	FVector PlayerStart = MyCharacter->GetActorLocation();		// 플레이어별 시작지점 지정
	PlayerStartLocation.Emplace(PlayerStart);							// Player1
	PlayerStart.Y += 300;
	PlayerStartLocation.Emplace(PlayerStart);							// Player2
	PlayerStart.X -= 300;
	PlayerStartLocation.Emplace(PlayerStart);							// Player3
	PlayerStart.Y -= 300;
	PlayerStartLocation.Emplace(PlayerStart);							// Player4
}

void APlayer_Manager::SpawnPlayers()
{
	UWorld* World = GetWorld();
	ASCoopGameMode* MyGameMode = Cast<ASCoopGameMode>(World->GetAuthGameMode());
	ASPlayerController* aiController = nullptr;

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
		
		ASCharacter* MyCharacter = Cast<ASCharacter>(Mycontroller->GetPawn());
		ASPlayerState* MyCharacterState = Cast< ASPlayerState>(MyCharacter->GetPlayerState());
		
		
		for (int i = 1; i < Playing; ++i) {
			ASCharacter* NewCharacter = World->SpawnActor<ASCharacter>(GenerateBp->GeneratedClass, PlayerStartLocation[i], FRotator::ZeroRotator, Spawnparams);
			NewCharacter->SetPlayerState(MyCharacterState);
			ASPlayerState* NewPlayerState = Cast< ASPlayerState>(NewCharacter->GetController()->PlayerState);
			if (NewPlayerState)
			{
				
				NewPlayerState->SetTeamNumber(MyCharacterState->GetTeamNumber());
			}
			if (aiController == nullptr)
			{
				aiController = Cast<ASPlayerController>(NewCharacter->GetController());
			}

			MyGameMode->SetOtherPlayerDefaults(NewCharacter);
			players.Emplace(NewCharacter);
		}
		
		if (!(PlayerId == 0)) 
			Mycontroller->Possess(players[PlayerId]);
		
		
		int num = 0;
		for (auto i = players.begin(); i != players.end(); ++i)
		{
			if (!((*i)->IsPlayerControlled()))
			{
				aiController->Possess(*i);
				(*i)->SetPlayerState(MyCharacterState);
			
			}
			(*i)->PlayerNum = num;
			++num;
		}
	}

	
		
}

TArray<ASCharacter*> APlayer_Manager::GetPlayerArray()
{
	return players;
}