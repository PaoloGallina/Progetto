

#include "stdafx.h"
#include "Sock.h"
#include "Datab.h"
#include "Oggetto.h"
#include "Folder.h"
#include <iostream>
#include <list>
#include <fcntl.h>
#include <io.h>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <string>
#include <chrono>
#include <exception>
#include <thread>
#include <mutex>

#define _CRTDBG_MAP_ALLOC

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

void TxtToList(SOCKET, list < Oggetto *>&);
void PulisciLista(std::list < Oggetto *>&);
void Sync(SOCKET client, std::string nome);
int ServeClient(SOCKET client);
list<string> clients;
mutex m;


int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock used to initiate the use of a DLL
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
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
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
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

	while (ClientSocket == INVALID_SOCKET){
		try{

			printf("SERVER i'm waiting for connection\n");
			// Accept a client socket
			ClientSocket = accept(ListenSocket, NULL, NULL);
			if (ClientSocket == INVALID_SOCKET) {
				printf("accept failed with error: %d\n", WSAGetLastError());
				throw "SERVER accept failed";
			}

			printf("SERVER a connection is settled\n");
			
			thread  cliente(ServeClient,ClientSocket);
			cliente.detach();
			ClientSocket = INVALID_SOCKET;
		}
		catch (char * a){
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	
	//questa parte del programma ||per ora|| non verrà mai eseguita
	//il server si tiene sempre online
	closesocket(ListenSocket);
	WSACleanup();
	_CrtDumpMemoryLeaks();
	return 0;
}

void TxtToList(SOCKET client,list < Oggetto *>& listaobj){

	printf("I get the number of obj in the list\n");
	int n = recInt(client);

	char* Hash = recvFile(client);
	wchar_t* PathNameLast = (wchar_t*)recvFile(client);
	istringstream c(Hash);
	wistringstream b(PathNameLast);

	
	int t = 0;
	while(t<n){
		wchar_t buf1[512], buf2[512], buf3[512];
		char buf4[512],buf5[512];
		b.getline(buf1, 512);
		b.getline(buf2, 512);
		b.getline(buf3, 512);
		c.getline(buf4, 512);
		listaobj.push_front(new Oggetto(buf1, buf2, buf3, buf4, *((DWORD *)recNbytes(client, sizeof(DWORD), buf5)), INVALID_HANDLE_VALUE));
		t++;
	}

	::free(Hash);
	::free(PathNameLast);
}

void PulisciLista(std::list < Oggetto *>& a){
	Oggetto* p2;
	while (!a.empty()){
		p2 = a.front();
		a.pop_front();
		delete p2;
		p2 = NULL;
	}
};

void Sync(SOCKET client, std::string nome){
	//qui dovrei passare il nome utente
	sqlite3 *db = CreateDatabase();
	std::list < Oggetto *> listaobj;
	TxtToList(client,listaobj); 
	std::list < Oggetto *> da_chiedere = CheFILEvoglio(db, listaobj);
	if (da_chiedere.size() != 0 || file_cancellati(db, listaobj.size()) != 0){
		nuovaVersione(db,client, listaobj, da_chiedere);
	}
	else{
		wcout << L"\nThe database is updated\n" << endl;
	}
	for (int i = max(1, GetUltimaVersione(db) - 2); i <= GetUltimaVersione(db); i++){
		ReadVERSIONE(db, i);
	}
	ReadFILES(db);
	PulisciLista(listaobj);
	PulisciLista(da_chiedere);
	sqlite3_close(db);
	//dico al client che la sync è terminata con successo
	sendInt(client, -10);
	printf("sync is terminated no more files are needed");

	this_thread::sleep_for(chrono::seconds(10));
	system("cls");

}

int ServeClient(SOCKET client){
	char* nome=nullptr;
	try{

		int sizename = recInt(client);
		nome= (char*)malloc(100 * sizeof(char));
		nome = (char*)recNbytes(client, sizename, nome);
		nome[sizename] = '\0';
		{

			lock_guard<mutex> LG(m);
			for (std::list<string>::iterator it = clients.begin(); it != clients.end(); ++it){
				string IT = *it;
				if (!IT.compare(nome)){
					sendInt(client, 999);
					::free(nome);
					closeConn(client);
					return 0;
				}
			}
			clients.push_back(nome);
			sendInt(client, 0);
		}

		while (1){
			int op = opRichiesta(client);
			if (op == 10){
				Sync(client, nome);
			}
			else if (op == 999){
				break;
			}
		}
	}
	catch (...){
		//Se non catchassi eccezioni a questo punto rischierei di far chrashare tutto il server
		lock_guard<mutex> LG(m);
		if (nome != nullptr)
			{clients.remove(nome);
			::free(nome);}
		closeConn(client);
		return -1;
	}
	
	lock_guard<mutex> LG(m);
	clients.remove(nome);
	::free(nome);
	closeConn(client);

	return 0;
}