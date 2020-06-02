#include "Server.h"
#include <stdio.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib") 

int id = 0;

SockInf g_clients[MAX_USER];
Player Player_Info;
R_Players recvplayer;
S_Players sendplayer;

bool IsStarted = false;
bool recving[MAX_USER] = { false };
bool LevelChange[MAX_USER] = { false };

void err_display(DWORD msg)
{
	LPVOID MsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&MsgBuf, 0, NULL);
	cout << "[" << msg << "] " << (char*)MsgBuf << endl;
	LocalFree(MsgBuf);
}

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	int clientId = reinterpret_cast<int>(overlapped->hEvent);

	// 클라이언트가 closesocket을 했을 경우
	if (Error != 0 || dataBytes == 0) {
		if (Error != 0) err_display(Error);
		printf("Error - LastError(error_code : %d)\n", WSAGetLastError());
		closesocket(g_clients[clientId].socket);
		cout << clientId + 1 << "번 플레이어 나감 (recv_callback)" << endl;
		g_clients[clientId].isUsed = false;
		Player_Info.IsUsed[clientId] = false;
		return;
	}

	Recv_Packet(clientId, g_clients[clientId].over.dataBuffer.buf);

	// &dataBytes -> 이거 리턴받는건데 비우는게 더 좋다고 말씀하셨음.
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD flags = 0;
	int retval;

	int clientId = reinterpret_cast<int>(overlapped->hEvent);

	// 클라이언트가 closesocket을 했을 경우
	if (dataBytes == 0) {
		printf("Error - LastError(error_code : %d)\n", WSAGetLastError());
		closesocket(g_clients[clientId].socket);
		cout << clientId + 1 << "번 플레이어 나감 (send_callback)" << endl;
		g_clients[clientId].isUsed = false;
		Player_Info.IsUsed[clientId] = false;
		return;
	}

	/*g_clients[clientId].over.dataBuffer.len = MAX_BUFFER;
	g_clients[clientId].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);

	memset(&(g_clients[clientId].over.overlapped), 0x00, sizeof(WSAOVERLAPPED));
	g_clients[clientId].over.overlapped.hEvent = (HANDLE)clientId;
*/

	if (!recving[clientId])
	{
		recving[clientId] = true;
		retval = WSARecv(g_clients[clientId].socket, &g_clients[clientId].over.dataBuffer, 1, 0, &flags,
			&(g_clients[clientId].over.overlapped), recv_callback);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
			}
		}
	}


	//WSARecv(g_clients[clientId].socket, &g_clients[clientId].over.dataBuffer, 1, 0, &flags,
	//	&(g_clients[clientId].over.overlapped), recv_callback);

}

int GetId()
{
	for (int i = 0; i < MAX_USER; ++i) {
		if (g_clients[i].isUsed == false) {
			g_clients[i].isUsed = true;
			return i;
		}
	}
	return -1;
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));

	listen(listenSocket, 5);
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	while (true) {
		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
		if (clientSocket != INVALID_SOCKET) {
			id = GetId();
			cout << id + 1 << "번 클라이언트 접속" << endl;
		}


		if (id < 0) {
			closesocket(clientSocket);
			continue;
		}

		memset(&g_clients[id], 0x00, sizeof(SockInf));
		g_clients[id].socket = clientSocket;
		g_clients[id].over.dataBuffer.len = sizeof(recvplayer);
		g_clients[id].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);
		ZeroMemory(&(g_clients[id].over.overlapped), sizeof(WSAOVERLAPPED));
		g_clients[id].clientId = id;
		g_clients[id].isUsed = true;
		flags = 0;

		for (int i = 0; i < MAX_USER; ++i) {
			if (true == g_clients[i].isUsed) {
				Player_Info.IsUsed[i] = g_clients[i].isUsed;
			}
		}

		g_clients[id].over.overlapped.hEvent = (HANDLE)id;


		// 1은 버퍼의 개수! 우리는 하나 쓸 것이다. 무턱대고 MAX쓰면 안 돼.
		// Recv 처리는 'recv_callback' 에서 한다.

		WSARecv(g_clients[id].socket, &g_clients[id].over.dataBuffer, 1, 0, &flags,
			&(g_clients[id].over.overlapped), recv_callback);
	}

	closesocket(listenSocket);
	WSACleanup();
}

void Recv_Packet(int clientId, char* buf) {

	R_Test* Test = reinterpret_cast<R_Test*>(buf);
	int retval;

	if (Test->packet_type == PACKET_CS_LOGIN) {
		cout << clientId + 1 << "번 플레이어 Login! " << endl;
		Player_Info.IsUsed[clientId] = true;
		if (Player_Info.Host == -1)
			Player_Info.Host = clientId;

		g_clients[clientId].over.dataBuffer.len = MAX_BUFFER;
		g_clients[clientId].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);

		memset(&(g_clients[clientId].over.overlapped), 0x00, sizeof(WSAOVERLAPPED));
		g_clients[clientId].over.overlapped.hEvent = (HANDLE)clientId;

		S_Login packet;
		packet.clientId = clientId;
		for (int i = 0; i < MAX_USER; ++i) {
			packet.Player[i] = Player_Info.IsUsed[i];  // 수정
			packet.Host = Player_Info.Host;
			cout << "플레이어 " << i + 1 << ": " << Player_Info.IsUsed[i] << endl;
		}
		cout << "방장 : 플레이어 " << Player_Info.Host << endl;

		for (int i = 0; i < MAX_USER; ++i) {
			if (Player_Info.IsUsed[i]) {

				OVER_EX* SendingOverlapped = new OVER_EX;

				SendingOverlapped->dataBuffer.len = sizeof(packet);
				memset(&(SendingOverlapped->overlapped), 0x00, sizeof(WSAOVERLAPPED));
				SendingOverlapped->overlapped.hEvent = (HANDLE)i;

				SendingOverlapped->dataBuffer.buf = reinterpret_cast<char*>(&packet);

				retval = WSASend(g_clients[i].socket, &(SendingOverlapped->dataBuffer), 1, NULL, 0,	 // 수정
					&(SendingOverlapped->overlapped), send_callback);
				if (retval == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSA_IO_PENDING)
					{
						printf("Error - %d CLINET Fail WSASend(error_code : %d)\n", i, WSAGetLastError());
					}
					else
					{
						delete SendingOverlapped;
					}
				}
				cout << i + 1 << "번 플레이어에게 로그인정보 전달!" << endl;
			}
		}

	}
	else {
		S_Players tmp;
		switch (Test->packet_type)
		{
		case PACKET_CS_GAME_START:
		{
			//R_Start* packet = reinterpret_cast<R_Start*>(buf);
			S_Start packet;
			IsStarted = true;
			packet.Started = IsStarted;
			cout << "호스트가 게임 시작함" << endl;

			g_clients[clientId].over.dataBuffer.len = MAX_BUFFER;
			g_clients[clientId].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);

			memset(&(g_clients[clientId].over.overlapped), 0x00, sizeof(WSAOVERLAPPED));
			g_clients[clientId].over.overlapped.hEvent = (HANDLE)clientId;

			for (int i = 0; i < MAX_USER; ++i) {
				if (Player_Info.IsUsed[i]) {

					OVER_EX* SendingOverlapped = new OVER_EX;

					SendingOverlapped->dataBuffer.len = sizeof(packet);
					memset(&(SendingOverlapped->overlapped), 0x00, sizeof(WSAOVERLAPPED));
					SendingOverlapped->overlapped.hEvent = (HANDLE)i;

					SendingOverlapped->dataBuffer.buf = reinterpret_cast<char*>(&packet);

					retval = WSASend(g_clients[i].socket, &(SendingOverlapped->dataBuffer), 1, NULL, 0,	 // 수정
						&(SendingOverlapped->overlapped), send_callback);
					if (retval == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSA_IO_PENDING)
						{
							printf("Error - %d CLINET Fail WSASend(error_code : %d)\n", i, WSAGetLastError());
						}
						else
						{
							delete SendingOverlapped;
						}
					}
					cout << i + 1 << "번 플레이어에게 게임시작 전달!" << endl;
				}
			}

		}
		break;
		case PACKET_CS_PLAYERS:
		{
			R_Players* packet = reinterpret_cast<R_Players*>(buf);
			///cout << clientId + 1 << "번 플레이어 정보 RECV !!!" << endl;

			Player_Info.HP[clientId] = packet->HP;
			Player_Info.Rot[clientId] = packet->Rot;
			Player_Info.Vel[clientId] = packet->Vel;
			Player_Info.Aim[clientId] = packet->Aim;
			Player_Info.IsJump[clientId] = packet->IsJump;
			Player_Info.isTargeting[clientId] = packet->isTargeting;
			Player_Info.IsSprinting[clientId] = packet->IsSprinting;
			Player_Info.onCrouchToggle[clientId] = packet->onCrouchToggle;
			Player_Info.WeaponState[clientId] = packet->WeaponState;

			g_clients[clientId].over.dataBuffer.len = MAX_BUFFER;
			g_clients[clientId].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);

			memset(&(g_clients[clientId].over.overlapped), 0x00, sizeof(WSAOVERLAPPED));
			g_clients[clientId].over.overlapped.hEvent = (HANDLE)clientId;

			/*cout << "x : " << Player_Info.Loc[clientId].x << " y : " << Player_Info.Loc[clientId].y << " z : " << Player_Info.Loc[clientId].z;*/
			/*cout << " Jump : " << (int)Player_Info.IsJump[clientId] << endl;*/
			/*cout << "Pitch : " << Player_Info.Rot[clientId].pitch << " Yaw : " << Player_Info.Rot[clientId].yaw
				<< " Roll : " << Player_Info.Rot[clientId].roll << endl;*/
				//cout << "VELOCITY - x : " << Player_Info.Vel[clientId].x << " y : " << Player_Info.Vel[clientId].y << " z : " << Player_Info.Vel[clientId].z << endl;

			S_Players s_packet;
			for (int i = 0; i < MAX_USER; ++i) {
				s_packet.Host = Player_Info.Host;
				s_packet.IsUsed[i] = Player_Info.IsUsed[i];
				s_packet.HP[i] = Player_Info.HP[i];
				s_packet.Rot[i] = Player_Info.Rot[i];
				s_packet.Vel[i] = Player_Info.Vel[i];
				s_packet.Aim[i] = Player_Info.Aim[i];
				s_packet.IsJump[i] = Player_Info.IsJump[i];
				s_packet.isTargeting[i] = Player_Info.isTargeting[i];
				s_packet.IsSprinting[i] = Player_Info.IsSprinting[i];
				s_packet.onCrouchToggle[i] = Player_Info.onCrouchToggle[i];
				s_packet.WeaponState[i] = Player_Info.WeaponState[i];
			}

			for (int i = 0; i < MAX_USER; ++i) {
				if (Player_Info.IsUsed[i]) {

					OVER_EX* SendingOverlapped = new OVER_EX;

					SendingOverlapped->dataBuffer.len = sizeof(s_packet);
					memset(&(SendingOverlapped->overlapped), 0x00, sizeof(WSAOVERLAPPED));
					SendingOverlapped->overlapped.hEvent = (HANDLE)i;

					SendingOverlapped->dataBuffer.buf = reinterpret_cast<char*>(&s_packet);

					retval = WSASend(g_clients[i].socket, &(SendingOverlapped->dataBuffer), 1, NULL, 0,	 // 수정
						&(SendingOverlapped->overlapped), send_callback);

					if (retval == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSA_IO_PENDING)
						{
							printf("Error - %d CLINET Fail WSASend(error_code : %d)\n", i, WSAGetLastError());
						}
						else
						{
							delete SendingOverlapped;
						}
					}
				}
			}

		}
		break;
		case PACKET_CS_LEVEL_CHANGE:
		{
			R_LevelChange* packet = reinterpret_cast<R_LevelChange*>(buf);
			cout << clientId + 1 << "번 플레이어 레벨 변경!" << endl;

			LevelChange[clientId] = packet->changed;

			g_clients[clientId].over.dataBuffer.len = MAX_BUFFER;
			g_clients[clientId].over.dataBuffer.buf = reinterpret_cast<char*>(&recvplayer);

			memset(&(g_clients[clientId].over.overlapped), 0x00, sizeof(WSAOVERLAPPED));
			g_clients[clientId].over.overlapped.hEvent = (HANDLE)clientId;

			S_LevelChange s_packet;

			int num = 0;
			int ischange = 0;
			for (int i = 0; i < MAX_USER; ++i)
			{
				if (g_clients[i].isUsed)
					++num;
			}
			for (int i = 0; i < MAX_USER; ++i)
			{
				if (LevelChange[i])
					++ischange;
			}
			if (num == ischange)
				s_packet.changed = true;
			else
				s_packet.changed = false;


			for (int i = 0; i < MAX_USER; ++i) {
				if (Player_Info.IsUsed[i]) {
					OVER_EX* SendingOverlapped = new OVER_EX;

					SendingOverlapped->dataBuffer.len = sizeof(s_packet);
					memset(&(SendingOverlapped->overlapped), 0x00, sizeof(WSAOVERLAPPED));
					SendingOverlapped->overlapped.hEvent = (HANDLE)i;

					SendingOverlapped->dataBuffer.buf = reinterpret_cast<char*>(&s_packet);

					retval = WSASend(g_clients[i].socket, &(SendingOverlapped->dataBuffer), 1, NULL, 0,	 // 수정
						&(SendingOverlapped->overlapped), send_callback);

					if (retval == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSA_IO_PENDING)
						{
							printf("Error - %d CLINET Fail WSASend(error_code : %d)\n", i, WSAGetLastError());
						}
						else
						{
							delete SendingOverlapped;
						}
					}
				}
			}
		}
		break;
		}
	}
	recving[clientId] = false;
}

void Send_Packet(char* buf) {

}