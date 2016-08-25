#include "stdafx.h"
#include <winsock2.h>
#include "DatabaseClass.h"
#include <iostream>
#include <list>
#include <fcntl.h>
#include <io.h>
#include <ws2tcpip.h>
#include <string>
#include <chrono>
#include <exception>
#include <thread>
#include "Sha.h"
#include <mutex>
#include <stdlib.h>
#include "Oggetto.h"
#include <Windows.h>
#define MAX_N_CLIENTS 10



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


DatabaseClass* Register(SOCKET client, std::string& nome, char* passw);
DatabaseClass* Login(SOCKET client, std::string& nome, char* passw);
int ServeClient(SOCKET client);
void SendLastconfig(SOCKET client, DatabaseClass* db, char* passw);
void SendConfig(SOCKET client, DatabaseClass* db, char* passw);
void SendAllFiles(SOCKET client, DatabaseClass* db, char* passw);
void TxtToList(SOCKET, std::list < Oggetto *>&, char* passw);
void PulisciLista(std::list < Oggetto *>&);
void Sync(SOCKET client, DatabaseClass* db, char* passw);

std::list<std::string> clients;
std::mutex m;

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	try{
		int temp;
		std::string porta;

		while (true){
			printf("Che porta vuoi utilizzare?\n");
			scanf("%d", &temp);
			if (temp > 0 && temp < 65535){
				break;
			}
			printf("Immetti una porta valida! Range 0-65535\n");
			system("pause");
			return -1;
		}
		printf("La porta utilizzata e' la numero %d\nNel caso la volessi modificare riavvia il server\n", temp);
		porta = std::to_string(temp);


		WSADATA wsaData;
		int iResult;

		
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
		iResult = getaddrinfo(NULL, porta.c_str(), &hints, &result);
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
	}
	catch (...){
		printf("Probabilmente hai aperto due volte il server senza accorgertene");
		system("pause");
		return -1;
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
			
			int tv=60000;
			  
			setsockopt(ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));	
			setsockopt(ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));

		    std::thread  cliente(ServeClient, ClientSocket);
			cliente.detach();
			
			//ServeClient( ClientSocket);
			ClientSocket = INVALID_SOCKET;
		
			
		}
		catch (...){
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

int ServeClient(SOCKET client) {
	std::string nome;
	
	char passw[64];
	DatabaseClass *db = NULL;
	try{
		int op = opRichiesta(client,nullptr);
		if (op == 30){
			db=Register(client, nome, passw);
		}
		else if (op == 40){
			db=Login(client, nome, passw);
		}
		else{
			closeConn(client);
			return -1;
		}
		

		while (1){
			
			op = opRichiesta(client,passw);
			if (op == 10){
				Sync(client, db, passw);
			}
			else if (op == 20){
				SendLastconfig(client, db,passw);
			}
			else if (op == 50){
				db->Restore(client,passw);
			}
			else if (op == 60){
				SendAllFiles(client, db,passw);
			}
			else if (op == 70){
				db->SendVersions(client,passw);
			}
			else if (op == 80){
				SendConfig(client, db,passw);
			}
			else if (op == 999){
				break;
			}
		}
	}
	catch (const char* e)
	{
		printf("\nlogout of a client due to an error\n");
		std::lock_guard<std::mutex> LG(m);
		if (strcmp(e, "user already served") != 0 && strcmp(e, "too many users") != 0){
			clients.remove(nome);
		}
		closeConn(client);
		delete db;
		return -1;
	}
	catch (...){
		printf("\nlogout of a client due to an error\n");
		std::lock_guard<std::mutex> LG(m);
		clients.remove(nome);
		closeConn(client);
		delete db;
		return -1;
	}

	std::lock_guard<std::mutex> LG(m);
	clients.remove(nome);
	closeConn(client);
	delete db;
	printf("logout of a client %s",nome.c_str());

	return 0;
}

void Sync(SOCKET client, DatabaseClass* db, char* passw){

	std::list < Oggetto *> newconfig,missingfiles;
	try{
		TxtToList(client, newconfig, passw);
		TxtToList(client, missingfiles, passw);

		if (missingfiles.size() != 0 || db->file_cancellati( newconfig.size()) != 0){
			std::wcout << L"\nA new version is insered\n" << std::endl;
			db->nuovaVersione( client, newconfig, missingfiles, passw);
		}
		else{
			//message to keep alive the connection
			sendInt(client, -10, passw); recInt(client, passw); recInt(client, passw); recInt(client, passw);
			std::wcout << L"\nThe database is updated\n" << std::endl;			
		}
	}
	catch (...){
		PulisciLista(newconfig);
		PulisciLista(missingfiles);
		throw "errore durante la sync";
	}
	PulisciLista(newconfig);
	PulisciLista(missingfiles);

	if (recInt(client, passw) != -10){
		printf("sync problem, not terminated correctly");
		throw "sync problem, not terminated correctly";
	}
	else{
		sendInt(client, -10, passw);
		printf("sync is terminated no more files are needed");
	}

	printf("\n\n\n");
}

void SendLastconfig(SOCKET client, DatabaseClass* db, char* passw){
	std::list<Oggetto*> last = db->LastVersion();
	try{


		//serialize list of files
		std::stringstream c;
		std::wstringstream b;

		for (std::list<Oggetto*>::iterator it = last.begin(); it != last.end(); ++it){
			Oggetto IT = *it;
			c.write(IT.GetHash().c_str(), IT.GetHash().size());
			c.write("\n", 1);
			b.write(IT.GetPath().c_str(), IT.GetPath().size());
			b.write(L"\n", 1);
			b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size());
			b.write(L"\n", 1);
		}
		printf("I send the number of files in the last config\n");
		sendInt(client, last.size(), passw);

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size(), passw);
		invFile(client, (char*)c.str().c_str(), c.str().size(), passw);

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t), passw);
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t), passw);

	}
	catch (...){
		PulisciLista(last);
		throw "error during send last config";
	}
	PulisciLista(last);
	sendInt(client, -20, passw);
}

void SendConfig(SOCKET client, DatabaseClass* db, char* passw){
	std::list<Oggetto*> last;
	try{
		last = db->Version( recInt(client, passw));

		//serialize list of files
		std::stringstream c;
		std::wstringstream b;

		for (std::list<Oggetto*>::iterator it = last.begin(); it != last.end(); ++it){
			Oggetto IT = *it;
			c.write(IT.GetHash().c_str(), IT.GetHash().size());
			c.write("\n", 1);
			b.write(IT.GetPath().c_str(), IT.GetPath().size());
			b.write(L"\n", 1);
			b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size());
			b.write(L"\n", 1);

		}
		printf("I send the number of files in the last config\n");
		sendInt(client, last.size(), passw);

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size(), passw);
		invFile(client, (char*)c.str().c_str(), c.str().size(), passw);

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t), passw);
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t), passw);

	}
	catch (...){
		PulisciLista(last);
		throw "error during sending the requested config";
	}
	PulisciLista(last);
	sendInt(client, -80, passw);
}

void SendAllFiles(SOCKET client, DatabaseClass* db, char* passw){
	std::list<Oggetto*> last = db->AllFiles();
	try{


		//serialize list of files
		std::stringstream c;
		std::wstringstream b;

		for (std::list<Oggetto*>::iterator it = last.begin(); it != last.end(); ++it){
			Oggetto IT = *it;
			c.write(IT.GetHash().c_str(), IT.GetHash().size());
			c.write("\n", 1);
			b.write(IT.GetPath().c_str(), IT.GetPath().size());
			b.write(L"\n", 1);
			b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size());
			b.write(L"\n", 1);

		}
		printf("I send the number of files in the last config\n");
		sendInt(client, last.size(),  passw);

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size(), passw);
		invFile(client, (char*)c.str().c_str(), c.str().size(), passw);

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t), passw);
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t), passw);

	}
	catch (...){
		PulisciLista(last);
		throw "error during send last config";
	}
	PulisciLista(last);
	sendInt(client, -60, passw);
}

void TxtToList(SOCKET client, std::list < Oggetto *>& listaobj,char* passw){

	char* Hash=nullptr;
	wchar_t* PathNameLast = nullptr;
	try{
		printf("I get the number of obj in the list\n");
		int n = recInt(client, passw);
		
		Hash = recvFile(client, passw);
		PathNameLast = (wchar_t*)recvFile(client, passw);
		std::istringstream c(Hash);
		std::wistringstream b(PathNameLast);


		int t = 0;
		while (t < n){
			wchar_t buf1[512], buf2[512], buf3[512];
			char buf4[512], buf5[512];
			b.getline(buf1, 512);
			b.getline(buf2, 512);
			b.getline(buf3, 512);
			c.getline(buf4, 512);
			listaobj.push_front(new Oggetto(buf1, buf2, buf3, buf4, *((DWORD *)recNbytes(client, sizeof(DWORD), buf5, passw)), INVALID_HANDLE_VALUE));
			t++;
		}
	}
	catch (...){
		::free(Hash);
		::free(PathNameLast);
		throw "errore generico";
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
		p2 = nullptr;
	}
};

DatabaseClass* Register(SOCKET client, std::string& nome, char* passw){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	DatabaseClass *db=NULL;

	int sizename = recInt(client,nullptr);
	nome = std::string((char*)recNbytes(client, sizename, recvbuf, nullptr));

	int sizepass = recInt(client,nullptr);
	std::string pass = std::string((char*)recNbytes(client, sizepass, recvbuf, nullptr));
	{
		std::lock_guard<std::mutex> LG(m);

		if (clients.size() > MAX_N_CLIENTS){
			sendInt(client, 999,nullptr);
			throw "too many users";
		}
		for (std::list<std::string>::iterator it = clients.begin(); it != clients.end(); ++it){
			std::string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999,nullptr);
				throw "user already served";
			}
		}
		clients.push_back(nome);
	}
	std::string temp = nome;
	temp.append(".db");
	std::ifstream my_file(temp);
	
	if (my_file)
	{
		sendInt(client, 999,nullptr);
		throw "User already registered ";
	}
	else{
		my_file.close();
		db = new DatabaseClass(nome);
		db->Register (pass);
				
	}
	memcpy(passw, pass.c_str(), 64);

	sendInt(client, -30,nullptr);
	return db;
};

DatabaseClass* Login(SOCKET client, std::string& nome, char* passw){
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	DatabaseClass *db = NULL;

	int sizename = recInt(client,nullptr);
	nome = std::string((char*)recNbytes(client, sizename, recvbuf, nullptr));


	{
		std::lock_guard<std::mutex> LG(m);
		
		if (clients.size() > MAX_N_CLIENTS){
			sendInt(client, 999,nullptr);
			throw "too many users";
		}
		for (std::list<std::string>::iterator it = clients.begin(); it != clients.end(); ++it){
			std::string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999,nullptr);
				throw "user already served";
			}
		}
		clients.push_back(nome);
		printf("\nSto servendo %s\n", nome.c_str());
	}
	sendInt(client, 0, nullptr);

	std::string temp = nome;
	temp.append(".db");
	std::ifstream my_file(temp);
	if (my_file)
	{	
		my_file.close();
		 db = new DatabaseClass(nome.c_str());	
		try{
			std::string passD = db->RetrivePass();

			int sfida1 = recInt(client, nullptr);
			std::string sfida = std::string(passD);
			sfida.append(std::to_string(sfida1));
			std::string hash = sha256(sfida);
			sendInt(client, hash.length() + 1, nullptr);
			sendNbytes(client, (char*)hash.c_str(), hash.length() + 1, nullptr);
			memcpy(passw, hash.c_str(), 64);

			int sfida2 = rand();
			sendInt(client, sfida2, nullptr);
			sfida = std::string(passD);
			sfida.append(std::to_string(sfida2));
			hash = sha256(sfida);

			int sizepass = recInt(client, nullptr);
			std::string ris_sfida = std::string((char*)recNbytes(client, sizepass, recvbuf, nullptr));
			if (hash.compare(ris_sfida) != 0){
				sendInt(client, 999,nullptr);
				throw "wrong password ";
			}
			
		}
		catch (...){
			throw "error during login";
		}
	}
	else{
		sendInt(client, 999,nullptr);
		throw "User not registered ";
	}
	
	
	sendInt(client, -40,nullptr);
	return db;
};

