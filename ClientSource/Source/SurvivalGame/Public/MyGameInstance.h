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
#define MAX_ZOMBIE 30
#define SERVERPORT 9000
#define MAX_BUFFER 2048

#define WEAPON_IDLE 10
#define WEAPON_FIRING 11
#define WEAPON_RELOADING 12

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

#define PACKET_SC_ZOMBIE_INIT 400
#define PACKET_SC_ZOMBIE 401

#define PACKET_CS_ZOMBIE_INIT 500
#define PACKET_CS_ZOMBIE 501

#define PACKET_SC_TIME 600
#define PACKET_CS_TIME 601

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

typedef struct ViewPoint
{
	Location Loc;
	Rotation Rot;
}ViewPoint;

typedef struct Info_Player {
	bool IsUsed[MAX_USER] = { false };
	int Host = -1;
	float HP[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER] = { false };
	bool IsTargeting[MAX_USER] = { false };
	bool IsSprinting[MAX_USER] = { false };
	bool onCrouchToggle[MAX_USER] = { false };
	//bool UseWeapon[MAX_USER] = { false };
	int WeaponState[MAX_USER] = { WEAPON_IDLE };
	ViewPoint View[MAX_USER];
}Player;

typedef struct Info_Zombie {
	float HP[MAX_ZOMBIE];
	bool IsAlive[MAX_ZOMBIE] = { false };
	bool Hit[MAX_ZOMBIE] = { false };
}Zombie;

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
	float HP;
	Rotation Rot;
	Velocity Vel;
	AimOffset Aim;
	bool IsJump;
	bool IsTargeting;
	bool IsSprinting;
	bool onCrouchToggle;
	//bool UseWeapon;
	int WeaponState;
	ViewPoint View;
}S_Players;

typedef struct Recv_Packet_Players {
	int packet_type = PACKET_SC_PLAYERS;
	int Host;
	bool IsUsed[MAX_USER];
	float HP[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER];
	bool IsTargeting[MAX_USER];
	bool IsSprinting[MAX_USER];
	bool onCrouchToggle[MAX_USER];
	//bool UseWeapon[MAX_USER];
	int WeaponState[MAX_USER];
	ViewPoint View[MAX_USER];
}R_Players;

typedef struct Send_Packet_Level_Change {
	int packet_type = PACKET_CS_LEVEL_CHANGE;
	bool changed;
}S_LevelChange;

typedef struct Recv_Packet_Level_Change {
	int packet_type = PACKET_SC_LEVEL_CHANGE;
	bool changed;
}R_LevelChange;


typedef struct Recv_Packet_Zombie
{
	int packet_type = PACKET_SC_ZOMBIE;
	bool IsAlive[MAX_ZOMBIE];
	float HP[MAX_ZOMBIE];
	bool Hit[MAX_ZOMBIE];
}R_Zombies;

typedef struct Send_Packet_Zombie
{
	int packet_type = PACKET_CS_ZOMBIE;
	bool IsAlive[MAX_ZOMBIE];
	float HP[MAX_ZOMBIE];
	bool Hit[MAX_ZOMBIE];
}S_Zombies;

typedef struct Send_Packet_Time
{
	int packet_type = PACKET_CS_TIME;
	int PlayerNum;
	int ElapsedTime = 0;
}S_Time;

typedef struct Recv_Packet_Time
{
	int packet_type = PACKET_SC_TIME;
	int ElapsedTime;
}R_Time;

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
static Zombie Zombie_info;
static int PlayerId;
static int Playing;
static FString Adress;
static bool Connected;
static int HostPlayer;
static bool GameStart;
static bool PlayerLogin;
static bool All_level_Changed;
static int Elapsed_Time;
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

	UFUNCTION(BlueprintCallable, Category = "TimeOfDay")
		bool IsAllLevelChange();

	~UMyGameInstance();
};
