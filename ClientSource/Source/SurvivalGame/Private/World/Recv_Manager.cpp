// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"
#include "Recv_Manager.h"

// Sets default values
ARecv_Manager::ARecv_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ARecv_Manager::BeginPlay()
{
	Super::BeginPlay();
	controller = GetWorld()->GetFirstPlayerController();
	
	SetActorTickInterval(0.16f);

	/*S_LevelChange s_packet;
	s_packet.changed = true;
	MySocket::sendBuffer(PACKET_CS_LEVEL_CHANGE, &s_packet);*/
}

// Called every frame
void ARecv_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//MySocket::RecvPacket();

	/*if (!All_level_Changed)
	{
		S_LevelChange s_packet;
		s_packet.changed = true;
		MySocket::sendBuffer(PACKET_CS_LEVEL_CHANGE, &s_packet);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("All Level Changed : %d "), All_level_Changed));
	}*/

}

