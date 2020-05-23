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

	SetActorTickInterval(0.033f);

	

	/*if (PlayerId > 0)
	{
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (Player_info.IsUsed[i])
			{
				FVector location;
				location.X = Player_info.Loc[i].x;
				location.Y = Player_info.Loc[i].y;
				location.Z = Player_info.Loc[i].z;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("New Player X : %f, Y : %f, Z : %f"),
					Player_info.Loc[i].x, Player_info.Loc[i].y, Player_info.Loc[i].z));
				FRotator rotation;
				rotation.Pitch = 0.0f;
				rotation.Yaw = 0.0f;
				rotation.Roll = 0.0f;
				ASCharacter* SpawnPlayer = SpawnActor<ASCharacter>(ASCharacter::StaticClass(), location, rotation);
				Array_Player[i] = SpawnPlayer;
			}
		}
		Array_Player[PlayerId]->AutoPossessPlayer = EAutoReceiveInput::Player0;
	}*/

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

		/*S_Players s_packet;
		s_packet.Loc = Player_info.Loc[PlayerId];
		s_packet.IsJump = Player_info.IsJump[PlayerId];
		MySocket::sendBuffer(PACKET_CS_PLAYERS, &s_packet);

		MySocket::RecvPacket();*/

		/*FVector location;
		location.X = Player_info.Loc[PlayerId].x + 15;
		location.Y = Player_info.Loc[PlayerId].y + 15;
		location.Z = Player_info.Loc[PlayerId].z;
		FRotator rotation;
		rotation.Yaw = 0.0f;
		rotation.Pitch = 0.0f;
		rotation.Roll = 0.0f;
*/
//ASCharacter* spawn = actorspawn(location, FRotator::ZeroRotator);
////ASCharacter* SpawnPlayer = SpawnActor<ASCharacter>(ASCharacter::StaticClass(), location, FRotator::ZeroRotator);
//if (spawn != NULL)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("SUCCESS! ")));
//}
//else 
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FAILED! ")));
//	//SpawnPlayer->GetRootComponent()->SetVisibility(true);
//}

/*FActorSpawnParameters SpawnInfo;
SpawnInfo.Instigator = Instigator;
APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn()->StaticClass(), location, rotation, SpawnInfo);*/

		//test = true;
	}


	if (!test)
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

		UWorld* World = GetWorld();
		FActorSpawnParameters Spawnparams;
		Spawnparams.Owner = this;
		Spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector loc;
		FRotator rot;
		loc.X = -4692.9672852;
		loc.Y = 4420.972168;
		loc.Z = 1132.0373535;

		ASCharacter* NewCharacter = World->SpawnActor<ASCharacter>(GenerateBp->GeneratedClass, loc, FRotator::ZeroRotator, Spawnparams);

		APlayerController* Mycontroller = World->GetFirstPlayerController();

		Mycontroller->Possess(NewCharacter);

		test = true;
	}
}

ASCharacter* APlayer_Manager::actorspawn(FVector Loc, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	ASCharacter* SpawnPlayer = GetWorld()->SpawnActor<ASCharacter>(ASCharacter::StaticClass(), Loc, Rot, SpawnParams);

	return SpawnPlayer;
}

ASCharacter** APlayer_Manager::Get_Player_Array()
{
	return Array_Player;
}

void APlayer_Manager::CheckNewPlayer()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (Player_info.IsUsed[i])
		{
			if (Array_Player[i] == NULL)
			{
				FVector location;
				location.X = Player_info.Loc[i].x;
				location.Y = Player_info.Loc[i].y;
				location.Z = Player_info.Loc[i].z;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("New Player X : %f, Y : %f, Z : %f"),
					Player_info.Loc[i].x, Player_info.Loc[i].y, Player_info.Loc[i].z));
				FRotator rotation;
				rotation.Pitch = 0.0f;
				rotation.Yaw = 0.0f;
				rotation.Roll = 0.0f;
				ASCharacter* SpawnPlayer = SpawnActor<ASCharacter>(ASCharacter::StaticClass(), location, rotation);
				Array_Player[i] = SpawnPlayer;
			}
		}
	}
}

