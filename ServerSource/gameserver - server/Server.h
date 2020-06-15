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
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER] = { false };
	bool isTargeting[MAX_USER] = { false };
	bool IsSprinting[MAX_USER] = { false };
	bool onCrouchToggle[MAX_USER] = { false };
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
	Rotation Rot[MAX_USER];
	Velocity Vel[MAX_USER];
	AimOffset Aim[MAX_USER];
	bool IsJump[MAX_USER];
	bool isTargeting[MAX_USER];
	bool IsSprinting[MAX_USER];
	bool onCrouchToggle[MAX_USER];
	int WeaponState[MAX_USER];
	ViewPoint View[MAX_USER];
}S_Players;

typedef struct Recv_Packet_Players {
	int packet_type = PACKET_CS_PLAYERS;
	float HP;
	Rotation Rot;
	Velocity Vel;
	AimOffset Aim;
	bool IsJump;
	bool isTargeting;
	bool IsSprinting;
	bool onCrouchToggle;
	int WeaponState;
	ViewPoint View;
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
	float HP[MAX_ZOMBIE];
	bool Hit[MAX_ZOMBIE];
}R_Zombies;

typedef struct Send_Packet_Zombie
{
	int packet_type = PACKET_SC_ZOMBIE;
	bool IsAlive[MAX_ZOMBIE];
	float HP[MAX_ZOMBIE];
	bool Hit[MAX_ZOMBIE];
}S_Zombies;



void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

int GetId();
void Recv_Packet(int clientId, char* buf);
void Send_Packet(char* buf);