#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include <thread>
#include <string>
#include <chrono>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

SOCKET __cdecl ConnectClient();
SOCKET  __cdecl ConnectServer();
void closeConn(SOCKET ConnectSocket);
void sendString(SOCKET ConnectSocket, char* stringa);
void sendInt(SOCKET ConnectSocket, int i);
void* recNbytes(SOCKET ConnectSocket, int size, char*stringa, int max);
int client();
int  server(void);
int opRichiesta(SOCKET Client);
char * recvFile(SOCKET Client);
void invFile(SOCKET Client, char*file, int size);
