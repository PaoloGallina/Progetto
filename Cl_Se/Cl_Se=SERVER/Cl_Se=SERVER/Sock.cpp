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


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"


SOCKET __cdecl ConnectClient()
{
	int argc = 2;
	char* addr = "127.0.0.1";
	

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char *sendbuf;
	int iResult;

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
			WSACleanup();
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	}
	printf("Connection is settled!\n");
	return ConnectSocket;
}

SOCKET  __cdecl ConnectServer(){
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	while (ClientSocket == INVALID_SOCKET){
		try{
			// Initialize Winsock used to initiate the use of a DLL
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				printf("WSAStartup failed with error: %d\n", iResult);
				throw "SERVER WSAStartup failed";
			}

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			// Resolve the server address and port
			iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed with error: %d\n", iResult);
				WSACleanup();
				throw "SERVER getaddrinfo failed ";
			}

			// Create a SOCKET for connecting to server
			ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (ListenSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				throw "SERVER socket failed \n";
			}

			// Setup the TCP listening socket
			iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(ListenSocket);
				WSACleanup();
				throw "SERVER bind failed  \n";
			}

			freeaddrinfo(result);

			iResult = listen(ListenSocket, SOMAXCONN);
			if (iResult == SOCKET_ERROR) {
				printf("listen failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				throw "SERVER listen failed";
			}
			printf("SERVER i'm waiting for connection\n");
			// Accept a client socket
			ClientSocket = accept(ListenSocket, NULL, NULL);
			if (ClientSocket == INVALID_SOCKET) {
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				throw "SERVER accept failed";
			}

			printf("SERVER a connection is settled\n");
			// No longer need server socket
			closesocket(ListenSocket);
		}
		catch (char * a){
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	return ClientSocket;
}

void closeConn(SOCKET ConnectSocket){
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		throw "shutdown failed with error: %d\n";
	}
}

void* recNbytes(SOCKET ConnectSocket, int size, char*stringa){
	int tot = 0;

	if (size > DEFAULT_BUFLEN){
		throw "requested too big packet";
	}

	while (tot < size){
		int iResult = recv(ConnectSocket, stringa + tot, size - tot, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
		{
			printf("Connection closing...\n");
			closeConn(ConnectSocket);
			return nullptr;
		}
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			throw "recv failed ";
		}
		tot += iResult;
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
			printf("Bytes sent: %d\n", iResult);
			tot += iResult;
		}
		else  {
			printf("sending failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup(); 
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
	return *ptr;
}

char * recvFile(SOCKET Client){
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	printf("I get the size of the file\n");
	int *ptr = (int*)recNbytes(Client, sizeof(int), recvbuf);
	int size = *ptr;
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
	char recvbuf[DEFAULT_BUFLEN];
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