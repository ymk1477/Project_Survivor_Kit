// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGame.h"
#include "MyGameInstance.h"

static FSocket* inst = nullptr;

FSocket& MySocket::getInstance() {
	return *inst;
}

void MySocket::initializeServer()
{
	inst = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("My_Server"), false);

	inst->SetNonBlocking();


	//FString ipaddress = TEXT("127.0.0.1");

	//FIPv4Address ip(127, 0, 0, 1);
	FIPv4Address ip;
	FIPv4Address::Parse(Adress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	addr->SetIp(ip.Value);
	addr->SetPort(SERVERPORT);

	bool connected = inst->Connect(*addr);

	if (!connected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail to Connect!! ")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Success to Connect!! ")));
		Connected = true;

	}

}

void MySocket::sendBuffer(int PacketType, void* BUF) {
	int32 zero = 0;
	bool Success;

	switch (PacketType) {
	case PACKET_CS_LOGIN:
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("S_Login Packet Process Begin!!")));
		S_Login* packet = reinterpret_cast<S_Login*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
	/*	if (Success) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("S_Login Send! SIZE : %d"), (int32)sizeof(*packet)));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("S_Login Packet Process Fail!!")));*/
	}
	break;
	case PACKET_CS_GAME_START:
	{
		S_Start* packet = reinterpret_cast<S_Start*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
		if (Success) {
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("S_Start Send! SIZE : %d"), (int32)sizeof(*packet)));
		}
	}
	break;
	case PACKET_CS_LOCATION:
	{
		S_Loc* packet = reinterpret_cast<S_Loc*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
		if (Success) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("s_obj SIZE : %d"), (int32)sizeof(*tmp)));
			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player X : %f, Y : %f, Z : %f "),
				packet->clientLoc.x, packet->clientLoc.y, packet->clientLoc.z));*/
		}
	}
	break;
	case PACKET_CS_JUMP:
	{
		S_Jump* packet = reinterpret_cast<S_Jump*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JUMP!")));
	}
	break;
	case PACKET_CS_PLAYERS:
	{
		S_Players* packet = reinterpret_cast<S_Players*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
		if(Success)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Complete to Send Player_Info !!")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail to Send Player_Info !!")));
	}
	break;
	case PACKET_CS_LEVEL_CHANGE:
	{
		S_LevelChange* packet = reinterpret_cast<S_LevelChange*>(BUF);
		Success = inst->Send((uint8*)packet, (int32)sizeof(*packet), zero);
	}
	break;
	}

	//MySocket::RecvPacket();
}

void MySocket::RecvPacket() {
	uint8* RECV_BUF = new uint8[MAX_BUFFER];
	R_Test* p_tmp;
	int32 zero = 0;

	bool Success = inst->Recv(RECV_BUF, (int32)MAX_BUFFER, zero);

	if (!Success) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail to Recv!! ")));
	}
	else {

		p_tmp = reinterpret_cast<R_Test*>(RECV_BUF);

		switch (p_tmp->packet_type)
		{
		case PACKET_SC_LOGIN:
		{
			Playing = 0;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Success to Recv LOGIN!! ")));
			R_Login* packet = reinterpret_cast<R_Login*>(RECV_BUF);
			if (PlayerId == -1)
				PlayerId = packet->clientId;
			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("PlayerId : %d "),
				PlayerId));*/
			if (HostPlayer == -1) {
				HostPlayer = packet->Host;
				Player_info.Host = HostPlayer;
			}
			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Host Player : %d "),
				HostPlayer));*/
			for (int i = 0; i < MAX_USER; ++i) {
				Player_info.IsUsed[i] = packet->Player[i];
				if (Player_info.IsUsed[i])
					Playing++;
			/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player Login : %d "),
					i + 1, packet->Player[i]));*/
			}

		}
		break;
		case PACKET_SC_GAME_START:
		{
			R_Start* packet = reinterpret_cast<R_Start*>(RECV_BUF);
			GameStart = packet->Started;
			/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Started : %d "), GameStart));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GAME START! ")));*/
		}
		break;
		case PACKET_SC_PLAYERS:
		{
			R_Players* packet = reinterpret_cast<R_Players*>(RECV_BUF);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Players RECV! ")));
			for (int i = 0; i < MAX_USER; ++i)
			{
				Player_info.Host = packet->Host;
				Player_info.IsUsed[i] = packet->IsUsed[i];
				if ( (i != PlayerId) && Player_info.IsUsed[i])
				{
					Player_info.Loc[i] = packet->Loc[i];
					Player_info.Rot[i] = packet->Rot[i];
					Player_info.IsJump[i] = packet->IsJump[i];
				}
				if (Player_info.IsUsed[i])
				{
					/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player -> X : %f, Y : %f, Z: %f "),
						i + 1, Player_info.Loc[i].x, Player_info.Loc[i].y, Player_info.Loc[i].z));*/
				/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d Player -> Pitch : %f, Yaw : %f, Roll : %f"),
						Player_info.Rot[PlayerId].pitch, Player_info.Rot[PlayerId].yaw, Player_info.Rot[PlayerId].roll));*/
				}
				
			}

		/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("X : %f, Y : %f, Z: %f "),
				Player_info.Loc[PlayerId].x, Player_info.Loc[PlayerId].y, Player_info.Loc[PlayerId].z));*/
		}
		break;
		case PACKET_SC_LEVEL_CHANGE:
		{
			R_LevelChange* packet = reinterpret_cast<R_LevelChange*>(RECV_BUF);
			All_level_Changed = packet->changed;
		}
		break;
		}

	}

	delete[] RECV_BUF;
}

UMyGameInstance::UMyGameInstance()
{
	PlayerId = -1;
	HostPlayer = -1;
	Connected = false;
	GameStart = false;
	PlayerLogin = false;
	All_level_Changed = false;
}

//Player UMyGameInstance::Get_Player_info()
//{
//	return Player_info;
//}
//
//void UMyGameInstance::Set_Player_Used(int id, bool used)
//{
//	Player_info.IsUsed[id] = used;
//}
//
//void UMyGameInstance::Set_Player_info(int id, FVector location)
//{
//	Player_info.Loc[id].x = location.X;
//	Player_info.Loc[id].y = location.Y;
//	Player_info.Loc[id].z = location.Z;
//}
//void UMyGameInstance::Set_Player_info(int id, bool jump)
//{
//	Player_info.IsJump[id] = jump;
//}
//int UMyGameInstance::Get_PlayerId()
//{
//	return PlayerId;
//}
//void UMyGameInstance::Set_PlayerId(int id)
//{
//	PlayerId = id;
//}
//FString UMyGameInstance::Get_Ip_Adress()
//{
//	return Adress;
//}
//void UMyGameInstance::Set_Ip_Adress(FString adress)
//{
//	Adress = adress;
//}
//bool UMyGameInstance::Get_Connected()
//{
//	return Connected;
//}
//int UMyGameInstance::Get_Host()
//{
//	return HostPlayer;
//}
//void UMyGameInstance::Set_Host(int host)
//{
//	HostPlayer = host;
//}
//bool UMyGameInstance::Get_GameStart()
//{
//	return GameStart;
//}
//void UMyGameInstance::Set_GameStart(bool start)
//{
//	GameStart = start;
//}
//bool UMyGameInstance::Get_PlayerLogin()
//{
//	return PlayerLogin;
//}
//void UMyGameInstance::Set_PlayerLogin(bool login)
//{
//	PlayerLogin = login;
//}

void UMyGameInstance::SetPlayer_Loc(float x, float y, float z)
{
	Player_info.Loc[PlayerId].x = x;
	Player_info.Loc[PlayerId].y = y;
	Player_info.Loc[PlayerId].z = z;
}

void UMyGameInstance::SetIpAdress(FString Ip_Adress)
{
	Adress = Ip_Adress;
}

bool UMyGameInstance::ConnectServer()
{
	MySocket::initializeServer();
	if (Connected)
		return true;
	else
		return false;
}

bool UMyGameInstance::IsConected()
{
	if (Connected)
		return true;
	else
		return false;
}

int UMyGameInstance::GetPlayer()
{
	return Playing;
}

bool UMyGameInstance::IsHost()
{
	if (PlayerId == HostPlayer)
		return true;
	else
		return false;
}

bool UMyGameInstance::IsStart()
{
	if (GameStart)
		return true;
	else
		return false;
}

void UMyGameInstance::Start()
{
	S_Start start;
	MySocket::sendBuffer(PACKET_CS_GAME_START, &start);
}

UMyGameInstance::~UMyGameInstance()
{
	if (Connected)
		inst->Close();
}