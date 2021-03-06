#pragma once
#include <iostream> 
#include <map> 
#include <WS2tcpip.h> 
#include <string>

using namespace std;

#define MAX_BUFFER 2048
#define SERVER_PORT 9000
#define MAX_USER 4
#define MAX_ZOMBIE 30

#define WEAPON_IDLE 10
#define WEAPON_FIRING 11
#define WEAPON_RERODING 12

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

#define PACKET_SC_ZOMBIE 401

#define PACKET_CS_ZOMBIE 501

#define PACKET_SC_TIME 600
#define PACKET_CS_TIME 601

#define PACKET_SC_COMBINE 700
#define PACKET_CS_COMBINE 701

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

typedef struct ViewPoint {
	Location Loc;
	Rotation Rot;
}ViewPoint;

struct OVER_EX {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
};

typedef struct SOCKETINFORM {
	OVER_EX over;
	SOCKET socket;
	int clientId;
	bool isUsed = false;
}SockInf;

typedef struct Info_Player {
	int Host = -1;
	bool IsUsed[MAX_USER] = { false };
	float HP[MAX_USER];
	Location Loc[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER] = { false };
	bool isTargeting[MAX_USER] = { false };
	bool IsSprinting[MAX_USER] = { false };
	bool onCrouchToggle[MAX_USER] = { false };
	int WeaponState[MAX_USER] = { WEAPON_IDLE };
	ViewPoint View[MAX_USER];
	int WeaponNum[MAX_USER] = { 1 };
	int Kit[MAX_USER] = { 0 };
	bool SirenButton = false;
	int ElapsedTime = 0;
}Player;

typedef struct Info_Zombie {
	float HP[MAX_ZOMBIE];
	bool IsAlive[MAX_ZOMBIE] = { true };
	int Target[MAX_ZOMBIE] = { -1 };
	Location Loc[MAX_ZOMBIE];
	//bool Hit[MAX_ZOMBIE] = { false };
}Zombie;

typedef struct Test_Packet {
	int packet_type;
	int i[256];
}R_Test;


typedef struct Recv_Packet_Location {
	int packet_type;
	Location clientLoc;
}R_Loc;

typedef struct Recv_Packet_Jump {
	int packet_type;
	int i;
}R_Jump;

typedef struct Recv_Packet_Login {
	int packet_type = PACKET_CS_LOGIN;
}R_Login;

typedef struct Send_Packet_Login {
	int packet_type = PACKET_SC_LOGIN;
	int clientId;
	bool Player[MAX_USER] = { false };
	int Host;
}S_Login;

typedef struct Send_Packet_GAME_START {
	int packet_type = PACKET_SC_GAME_START;
	bool Started;
}S_Start;

typedef struct Recv_Packet_GAME_START {
	int packet_type = PACKET_CS_GAME_START;
}R_Start;

typedef struct Send_Packet_Players {
	int packet_type = PACKET_SC_PLAYERS;
	int Host;
	bool IsUsed[MAX_USER];
	float HP[MAX_USER];
	Location Loc[MAX_USER];
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER];
	bool isTargeting[MAX_USER];
	bool IsSprinting[MAX_USER];
	bool onCrouchToggle[MAX_USER];
	int WeaponState[MAX_USER];
	ViewPoint View[MAX_USER];
	int WeaponNum[MAX_USER];
	int Kit[MAX_USER];
	bool SirenButton;
	int ElapsedTime;
	bool ZombieIsAlive[MAX_ZOMBIE];
	int ZombieTarget[MAX_ZOMBIE];
	//Location ZombieLoc[MAX_ZOMBIE];
	//int ZombieHP[MAX_ZOMBIE];
}S_Players;

typedef struct Recv_Packet_Players {
	int packet_type = PACKET_CS_PLAYERS;
	float HP;
	Location Loc;
	Rotation Rot;
	Velocity Vel;
	AimOffset Aim;
	bool IsJump;
	bool isTargeting;
	bool IsSprinting;
	bool onCrouchToggle;
	int WeaponState;
	ViewPoint View;
	int WeaponNum;
	int Kit;
	bool SirenButton;
	int ElapsedTime;
	bool ZombieIsAlive[MAX_ZOMBIE];
	int ZombieTarget[MAX_ZOMBIE];
	//Location ZombieLoc[MAX_ZOMBIE];
	//int ZombieHP[MAX_ZOMBIE];
}R_Players;

typedef struct Recv_Packet_Level_Change {
	int packet_type = PACKET_CS_LEVEL_CHANGE;
	bool changed;
}R_LevelChange;

typedef struct Send_Packet_Level_Change {
	int packet_type = PACKET_SC_LEVEL_CHANGE;
	bool changed;
}S_LevelChange;

typedef struct Recv_Packet_Zombie
{
	int packet_type = PACKET_CS_ZOMBIE;
	bool IsAlive[MAX_ZOMBIE];
	int Target[MAX_ZOMBIE] = { -1 };
	float HP[MAX_ZOMBIE];
	Location Loc[MAX_ZOMBIE];
	//bool Hit[MAX_ZOMBIE];
}R_Zombies;

typedef struct Send_Packet_Zombie
{
	int packet_type = PACKET_SC_ZOMBIE;
	bool IsAlive[MAX_ZOMBIE];
	int Target[MAX_ZOMBIE] = { -1 };
	float HP[MAX_ZOMBIE];
	Location Loc[MAX_ZOMBIE];
	//bool Hit[MAX_ZOMBIE];`
}S_Zombies;

typedef struct Recv_Packet_Time
{
	int packet_type = PACKET_CS_TIME;
	int PlayerNum;
	int ElapsedTime;
}R_Time;

typedef struct Send_Packet_Time
{
	int packet_type = PACKET_SC_TIME;
	int ElapsedTime;
}S_Time;

typedef struct Send_Packet_Combine
{
	int packet_type = PACKET_SC_COMBINE;
	S_Players player;
	S_Zombies zombie;
	S_Time time;
}S_Combine;

typedef struct Recv_Packet_Combine
{
	int packet_type = PACKET_CS_COMBINE;
	R_Players player;
	R_Zombies zombie;
	R_Time time;
}R_Combine;

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

int GetId();
void Recv_Packet(int clientId, char* buf);
void Send_Packet(char* buf);