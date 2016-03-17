#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
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



#define DEFAULT_BUFLEN 1500
#define DEFAULT_PORT "8080"

SOCKET __cdecl ConnectClient(HANDLE hpipe);
void closeConn(SOCKET ConnectSocket);
void sendNbytes(SOCKET ConnectSocket, char*stringa, int size, char* pass);
void sendInt(SOCKET ConnectSocket, int i, char* pass);
void* recNbytes(SOCKET ConnectSocket, int size, char*stringa, char* pass);
int opRichiesta(SOCKET Client,  char* pass);
char * recvFile(SOCKET Client,  char* pass);
void invFile(SOCKET Client, char*file, int size,  char* pass);
int recInt(SOCKET client,  char* pass);