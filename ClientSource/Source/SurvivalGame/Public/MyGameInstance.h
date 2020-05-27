// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Engine/GameInstance.h"

#include "MyGameInstance.generated.h"

#define MAX_USER 4
#define SERVERPORT 9000
#define MAX_BUFFER 2048

#define PACKET_CS_GAME_START 300
#define PACKET_SC_GAME_START 301
#define PACKET_CS_LEVEL_CHANGE 302
#define PACKET_SC_LEVEL_CHANGE 303

#define PACKET_SC_LOGIN 100
#define PACKET_SC_LOCATION 101
#define PACKET_SC_JUMP 102
#define PACKET_SC_PLAYERS 103

#define PACKET_CS_LOGIN 200
#define PACKET_CS_LOCATION 201
#define PACKET_CS_JUMP 202
#define PACKET_CS_PLAYERS 203

typedef struct LOCATION {
	float x;
	float y;
	float z;
}Location;

typedef struct Rotation {
	float yaw;
	float pitch;
	float roll;
}Rotation;

typedef struct Velocity {
	float x;
	float y;
	float z;
}Velocity;

typedef struct AimOffset {
	float yaw;
	float pitch;
	float roll;
}AimOffset;

typedef struct Info_Player {
	bool IsUsed[MAX_USER] = { false };
	int Host = -1;
	Location Loc[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER] = { false };
	bool IsTargeting[MAX_USER] = { false };
	bool IsSprinting[MAX_USER] = { false };
	bool onCrouchToggle[MAX_USER] = { false };
}Player;

typedef struct Test_Packet {
	int packet_type;
	int i;
}R_Test;

typedef struct Send_Packet_Location {
	int packet_type = PACKET_CS_LOCATION;
	Location clientLoc;
}S_Loc;

typedef struct Send_Packet_Jump {
	int packet_type = PACKET_CS_JUMP;
	int i;
}S_Jump;

typedef struct Send_Packet_Login {
	int packet_type = PACKET_CS_LOGIN;
}S_Login;

typedef struct Recv_Packet_Login {
	int packet_type = PACKET_SC_LOGIN;
	int clientId;
	bool Player[MAX_USER] = { false };
	int Host;
}R_Login;

typedef struct Send_Packet_GAME_START {
	int packet_type = PACKET_CS_GAME_START;
}S_Start;

typedef struct Recv_Packet_GAME_START {
	int packet_type = PACKET_SC_GAME_START;
	bool Started;
}R_Start;

typedef struct Send_Packet_Players {
	int packet_type = PACKET_CS_PLAYERS;
	Location Loc;
	Rotation Rot;
	Velocity Vel;
	bool IsJump;
	bool IsTargeting;
	bool IsSprinting;
	bool onCrouchToggle;
}S_Players;

typedef struct Recv_Packet_Players {
	int packet_type = PACKET_SC_PLAYERS;
	int Host;
	bool IsUsed[MAX_USER];
	Location Loc[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	
	bool IsJump[MAX_USER];
	bool IsTargeting[MAX_USER];
	bool IsSprinting[MAX_USER];
	bool onCrouchToggle[MAX_USER];

}R_Players;

typedef struct Send_Packet_Level_Change {
	int packet_type = PACKET_CS_LEVEL_CHANGE;
	bool changed;
}S_LevelChange;

typedef struct Recv_Packet_Level_Change {
	int packet_type = PACKET_SC_LEVEL_CHANGE;
	bool changed;
}R_LevelChange;


class MySocket {
private:
	MySocket() {}
public:
	static FSocket& getInstance();

	static void initializeServer();
	static void sendBuffer(int PacketType, void* BUF);
	static void RecvPacket();


};

static Player Player_info;
static int PlayerId;
static int Playing;
static FString Adress;
static bool Connected;
static int HostPlayer;
static bool GameStart;
static bool PlayerLogin;
static bool All_level_Changed;

/**
 *
 */
UCLASS()
class SURVIVALGAME_API UMyGameInstance : public UGameInstance
{

	GENERATED_BODY()

private:

public:

	UMyGameInstance();

	void SetPlayer_Loc(float x, float y, float z);

	/*Player Get_Player_info();
	void Set_Player_Used(int id, bool used);
	void Set_Player_info(int id, FVector location);
	void Set_Player_info(int id, bool jump);
	int Get_PlayerId();
	void Set_PlayerId(int id);
	FString Get_Ip_Adress();
	void Set_Ip_Adress(FString adress);
	bool Get_Connected();
	int Get_Host();
	void Set_Host(int host);
	bool Get_GameStart();
	void Set_GameStart(bool start);
	bool Get_PlayerLogin();
	void Set_PlayerLogin(bool login);*/


	UFUNCTION(BlueprintCallable, Category = "My_Server")
		void SetIpAdress(FString Ip_Adress);

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		bool ConnectServer();

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		bool IsConected();

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		int GetPlayer();

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		bool IsHost();

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		bool IsStart();

	UFUNCTION(BlueprintCallable, Category = "My_Server")
		void Start();

	~UMyGameInstance();
};
