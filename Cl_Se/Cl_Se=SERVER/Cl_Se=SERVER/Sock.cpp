#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include <string>
#include <chrono>
#include <exception>
#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8080"


SOCKET __cdecl ConnectClient(HANDLE hpipe)
{

	int c;
	DWORD read;
	DWORD NuByRe;
	char addr[100];

	ReadFile(hpipe, addr, 4, &read, NULL);
	c = *((int*)addr);
	ReadFile(hpipe, addr, c, &read, NULL);
	addr[read] = '\0';

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;

	int flag = 3;
	while (ConnectSocket == INVALID_SOCKET){
		try{


			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				printf("WSAStartup failed with error: %d\n", iResult);
				throw "WSAStartup failed with error: %d\n";
			}

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			// Resolve the server address and port
			iResult = getaddrinfo(addr, DEFAULT_PORT, &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed with error: %d\n", iResult);
				throw "getaddrinfo failed with error: %d\n";
			}

			// Attempt to connect to an address until one succeeds
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

				// Create a SOCKET for connecting to server
				ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (ConnectSocket == INVALID_SOCKET) {
					printf("socket failed with error: %ld\n", WSAGetLastError());
					throw "socket failed with error: %ld\n";
				}

				// Connect to server.
				iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (iResult == SOCKET_ERROR) {
					closesocket(ConnectSocket);
					ConnectSocket = INVALID_SOCKET;
					continue;
				}
				break;
			}


			freeaddrinfo(result);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("Unable to connect to server!\n");
				WSACleanup();
				throw "Unable to connect to server!\n";
			}
		}
		catch (char* a){
			printf("I'll retry in a while\n");
			flag--;
			if (flag < 1){
				break;
			}
		}
	}
	
	if (ConnectSocket != INVALID_SOCKET){
		printf("Connection is settled!\n");
		WriteFile(hpipe, L"OK\n", 3 * sizeof(wchar_t), &NuByRe, NULL);

	}

	struct timeval tv;
	tv.tv_sec = 30000;  /* 30 Secs Timeout */
	setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
	setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));

	return ConnectSocket;
}

void closeConn(SOCKET ConnectSocket){
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
	}
	closesocket(ConnectSocket);
}

void* recNbytes(SOCKET ConnectSocket, int size, char*stringa){
	int tot = 0;

	if (size > DEFAULT_BUFLEN){
		throw "requested too big packet";
	}

	while (tot < size){
		int iResult = recv(ConnectSocket, stringa + tot, size - tot, 0);
		tot += iResult;
		if (iResult > 0 ) {
			//DEBUG
			//printf("Bytes received: %d TOT=%d\n", iResult,tot);
		}
		else if (iResult == 0)
		{
			printf("Connection closing, this should not happen...\nI was waiting for som data and you closed the conn!\n");
			throw "shutdown by the peer not expected!";
		}
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			throw "recv failed ";
		}
	}
	return stringa;
}

void sendNbytes(SOCKET ConnectSocket,char*stringa, int size){
	int tot = 0;

	if (size > DEFAULT_BUFLEN){
		throw "too big packet";
	}

	while (tot < size){
		int iResult = send(ConnectSocket,stringa+tot, size - tot, 0);
		if (iResult > 0) {
			//DEBUG
			// printf("Bytes sent: %d\n", iResult);
			tot += iResult;
		}
		else  {
			printf("sending failed with error: %d\n", WSAGetLastError());
			throw "send failed ";
		}
	}
}

void sendInt(SOCKET ConnectSocket, int i){
	char* buf;
	buf = (char*)&i;
	sendNbytes(ConnectSocket, buf, 4);
}

int recInt(SOCKET client){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int *ptr = (int*)recNbytes(client, sizeof(int), recvbuf);
	int size = *ptr;
	return size;
}

int opRichiesta(SOCKET Client){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int *ptr = (int*)recNbytes(Client, sizeof(int), recvbuf);
	int size = *ptr;
	return size;
}

char* recvFile(SOCKET Client){
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	printf("I get the size of the file\n");
	int size = recInt(Client);
	char* file = (char*)malloc(size*sizeof(char)+4);
	
	int tot = 0;
	
	while (tot < size){
		if (tot + recvbuflen < size){
			memcpy(file + tot, (char*)recNbytes(Client, recvbuflen, recvbuf), recvbuflen);
		}
		else{
			memcpy(file + tot, (char*)recNbytes(Client, size - tot, recvbuf), size - tot);
			tot += size - tot;
			break;
		}
		tot += DEFAULT_BUFLEN;
	}
	
	file[tot] = L'\0';
	return file;
}

void invFile(SOCKET Client, char*file,int size){
	int recvbuflen = DEFAULT_BUFLEN;
	int tot = 0;

	while (tot < size){
		if (tot + recvbuflen < size){
			sendNbytes(Client, file + tot, recvbuflen);
		}
		else{
			sendNbytes(Client, file + tot, size - tot);
		}
		tot += DEFAULT_BUFLEN;
	}
}

