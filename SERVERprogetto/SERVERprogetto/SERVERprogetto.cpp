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
#include "Sha.h"
#include <mutex>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

void TxtToList(SOCKET, std::list < Oggetto *>&);
void PulisciLista(std::list < Oggetto *>&);
void Sync(SOCKET client, std::string nome);
void Register(SOCKET client, string& nome);
void Login(SOCKET client,string& nome);
int ServeClient(SOCKET client);
void SendLastconfig(SOCKET client, std::string nome);
list<string> clients;
mutex m;


int _tmain(int argc, _TCHAR* argv[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
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
		//	ServeClient( ClientSocket);
			ClientSocket = INVALID_SOCKET;
		//	break;
			
		}
		catch (char * a){
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	
	//questa parte del programma ||per ora|| non verrà mai eseguita
	//il server si tiene sempre online
	closesocket(ListenSocket);
	WSACleanup();
	//_CrtDumpMemoryLeaks();
	return 0;
}

int ServeClient(SOCKET client) {
	string nome;
	try{
		int op = opRichiesta(client);
		if (op == 30){
			Register(client,nome);
		}
		else if (op == 40){
			Login(client,nome);
		}
		else{
			closeConn(client);
			return -1;
		}
		

		while (1){
			op = opRichiesta(client);
			if (op == 10){
				Sync(client, nome);
			}
			else if (op == 20){
				SendLastconfig(client, nome);
			}
			else if (op == 999){
				break;
			}
		}
	}
	catch (...){
		printf("logout of a client due to an error");
		lock_guard<mutex> LG(m);
		if (nome.length()!=0)
		{
			clients.remove(nome);
			
		}
		closeConn(client);
		return -1;
	}

	lock_guard<mutex> LG(m);
	clients.remove(nome);
	closeConn(client);
	printf("logout of a client");
	return 0;
}

void Sync(SOCKET client, std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	std::list < Oggetto *> newconfig,missingfiles;

	TxtToList(client, newconfig);
	TxtToList(client, missingfiles);

	
	if (missingfiles.size() != 0 || file_cancellati(db, newconfig.size()) != 0){
		nuovaVersione(db, client, newconfig, missingfiles);
	}
	else{
		wcout << L"\nThe database is updated\n" << endl;
	}
	for (int i = max(1, GetUltimaVersione(db) - 2); i <= GetUltimaVersione(db); i++){
		ReadVERSIONE(db, i);
	}
	ReadFILES(db);
	PulisciLista(newconfig);
	PulisciLista(missingfiles);
	sqlite3_close(db);
	//dico al client che la sync è terminata con successo
	sendInt(client, -10);
	if (recInt(client) != -10){
		printf("sync problem, not terminated correctly");
	}
	else{
		sendInt(client, -10);
		printf("sync is terminated no more files are needed");
	}

	this_thread::sleep_for(chrono::seconds(10));
	system("cls");

}

void SendLastconfig(SOCKET client,  std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	list<Oggetto*> last = LastVersion(db);
	
	//serialize list of files
	stringstream c;
	wstringstream b;

	for (std::list<Oggetto*>::iterator it = last.begin(); it != last.end(); ++it){
		Oggetto IT = *it;
		c.write(IT.GetHash().c_str(), IT.GetHash().size());
		c.write("\n", 1);
		b.write(IT.GetPath().c_str(), IT.GetPath().size());
		b.write(L"\n", 1);
	}
	printf("I send the number of files in the last config\n");
	sendInt(client, last.size());

	printf("I send the size of first file and the file\n");
	sendInt(client, c.str().size());
	invFile(client, (char*)c.str().c_str(), c.str().size());

	printf("I send the size of second file and the file\n");
	sendInt(client, b.str().size()*sizeof(wchar_t));
	invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	PulisciLista(last);
	sqlite3_close(db);
	sendInt(client,-20);
}

void TxtToList(SOCKET client, list < Oggetto *>& listaobj){

	printf("I get the number of obj in the list\n");
	int n = recInt(client);

	char* Hash = recvFile(client);
	wchar_t* PathNameLast = (wchar_t*)recvFile(client);
	istringstream c(Hash);
	wistringstream b(PathNameLast);


	int t = 0;
	while (t<n){
		wchar_t buf1[512], buf2[512], buf3[512];
		char buf4[512], buf5[512];
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

void Register(SOCKET client,string& nome){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	int sizename = recInt(client);
	nome = string((char*)recNbytes(client, sizename, recvbuf));

	int sizepass = recInt(client);
	string pass = sha256(string((char*)recNbytes(client, sizepass, recvbuf)));
	
	{

		lock_guard<mutex> LG(m);
		for (std::list<string>::iterator it = clients.begin(); it != clients.end(); ++it){
			string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999);
				closeConn(client);
				throw "user already served";
			}
		}
		clients.push_back(nome);
	}
	
	ifstream my_file(nome);
	if (my_file)
	{
		sendInt(client, 999);
		throw "User already registered ";
	}
	else{
		my_file.close();
		sqlite3 *db = CreateDatabase(nome.c_str());
		int  rc;
		std::string sql = "INSERT INTO CREDENTIAL (USER,PASS) VALUES (?1, ?2);";

		sqlite3_stmt* stm = NULL;
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

		rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, pass.c_str(), pass.size(), SQLITE_STATIC);

		rc = sqlite3_step(stm);
		rc = sqlite3_finalize(stm);
	}

	sendInt(client, -30);
	return;
};

void Login(SOCKET client,string& nome){
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	int sizename = recInt(client);
	nome = string((char*)recNbytes(client, sizename, recvbuf));

	int sizepass = recInt(client);
	string pass = sha256(string((char*)recNbytes(client, sizepass, recvbuf)));

	{

		lock_guard<mutex> LG(m);
		for (std::list<string>::iterator it = clients.begin(); it != clients.end(); ++it){
			string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999);
				closeConn(client);
				throw "user already served";
			}
		}
		clients.push_back(nome);
	}

	ifstream my_file(nome);
	if (my_file)
	{	
		my_file.close();
		sqlite3 *db = CreateDatabase(nome.c_str());
		std::string sql = "SELECT PASS FROM CREDENTIAL WHERE USER=?1";
		sqlite3_stmt* stm;
		int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
		rc = sqlite3_step(stm);
		string passD=string ((char*)sqlite3_column_text(stm, 0));
		if (passD.compare(pass)!=0){
			sendInt(client, 999);
			throw "wrong password ";
		}
		rc = sqlite3_finalize(stm);
	}
	else{
		sendInt(client, 999);
		throw "User not registered ";
	}
	
	sendInt(client, -40);
	
};