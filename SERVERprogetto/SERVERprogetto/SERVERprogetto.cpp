#include "stdafx.h"
#include "Sock.h"
#include "Datab.h"
#include "Oggetto.h"
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
#include <stdlib.h>
#define MAX_N_CLIENTS 10



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

void TxtToList(SOCKET, std::list < Oggetto *>&);
void PulisciLista(std::list < Oggetto *>&);
void Sync(SOCKET client, std::string nome);
void Restore(SOCKET client, std::string nome);
void Register(SOCKET client, std::string& nome);
void Login(SOCKET client,std::string& nome);
int ServeClient(SOCKET client);
void SendLastconfig(SOCKET client, std::string nome);
void SendConfig(SOCKET client, std::string nome);
void SendAllFiles(SOCKET client, std::string nome);

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
		catch (char * a){
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

int ServeClient(SOCKET client) {
	std::string nome;
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
			else if (op == 50){
				Restore(client, nome);
			}
			else if (op == 60){
				SendAllFiles(client, nome);
			}
			else if (op == 70){
				SendVersions(client, nome);
			}
			else if (op == 80){
				SendConfig(client, nome);
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
		return -1;
	}
	catch (...){
		printf("\nlogout of a client due to an error\n");
		std::lock_guard<std::mutex> LG(m);
		clients.remove(nome);
		closeConn(client);
		return -1;
	}

	std::lock_guard<std::mutex> LG(m);
	clients.remove(nome);
	closeConn(client);
	printf("logout of a client %s",nome.c_str());

	return 0;
}

void Sync(SOCKET client, std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	std::list < Oggetto *> newconfig,missingfiles;
	try{
		TxtToList(client, newconfig);
		TxtToList(client, missingfiles);

		if (missingfiles.size() != 0 || file_cancellati(db, newconfig.size()) != 0){
			std::wcout << L"\nA new version is insered\n" << std::endl;
			nuovaVersione(db, client, newconfig, missingfiles);
			//sqlite3_exec(db, "vacuum;", NULL, NULL, NULL);
		}
		else{
			//message to keep alive the connection
			sendInt(client, -10); recInt(client); recInt(client); recInt(client);
			std::wcout << L"\nThe database is updated\n" << std::endl;			
		}
	}
	catch (...){
		PulisciLista(newconfig);
		PulisciLista(missingfiles);
		int rc=sqlite3_close(db);
		throw "errore durante la sync";
	}
	PulisciLista(newconfig);
	PulisciLista(missingfiles);
	sqlite3_close(db);

	if (recInt(client) != -10){
		printf("sync problem, not terminated correctly");
		throw "sync problem, not terminated correctly";
	}
	else{
		sendInt(client, -10);
		printf("sync is terminated no more files are needed");
	}

	printf("\n\n\n");
}

void SendLastconfig(SOCKET client,  std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	std::list<Oggetto*> last = LastVersion(db);
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
		sendInt(client, last.size());

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size());
		invFile(client, (char*)c.str().c_str(), c.str().size());

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t));
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));

	}
	catch (...){
		PulisciLista(last);
		sqlite3_close(db);
		throw "error during send last config";
	}
	PulisciLista(last);
	sqlite3_close(db);
	sendInt(client,-20);
}

void SendConfig(SOCKET client, std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	std::list<Oggetto*> last;
	try{
		last = Version(db, recInt(client));

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
		sendInt(client, last.size());

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size());
		invFile(client, (char*)c.str().c_str(), c.str().size());

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t));
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));

	}
	catch (...){
		PulisciLista(last);
		sqlite3_close(db);
		throw "error during sending the requested config";
	}
	PulisciLista(last);
	sqlite3_close(db);
	sendInt(client, -80);
}

void SendAllFiles(SOCKET client, std::string nome){
	sqlite3 *db = CreateDatabase(nome);
	std::list<Oggetto*> last = AllFiles(db);
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
		sendInt(client, last.size());

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size());
		invFile(client, (char*)c.str().c_str(), c.str().size());

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size()*sizeof(wchar_t));
		invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));

	}
	catch (...){
		PulisciLista(last);
		sqlite3_close(db);
		throw "error during send last config";
	}
	PulisciLista(last);
	sqlite3_close(db);
	sendInt(client, -60);
}

void TxtToList(SOCKET client, std::list < Oggetto *>& listaobj){

	char* Hash=nullptr;
	wchar_t* PathNameLast = nullptr;
	try{
		printf("I get the number of obj in the list\n");
		int n = recInt(client);

		Hash = recvFile(client);
		PathNameLast = (wchar_t*)recvFile(client);
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
			listaobj.push_front(new Oggetto(buf1, buf2, buf3, buf4, *((DWORD *)recNbytes(client, sizeof(DWORD), buf5)), INVALID_HANDLE_VALUE));
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

void Restore(SOCKET client, std::string nome){

	char hash[DEFAULT_BUFLEN];
	wchar_t path[DEFAULT_BUFLEN];

	int Lhash = recInt(client);
	recNbytes(client, Lhash, hash);
	int Lpath = recInt(client);
	recNbytes(client, Lpath, (char*)path);

	int rc;
	sqlite3* db = CreateDatabase(nome);
	sqlite3_blob *BLOB = nullptr;
	sqlite3_stmt* stm = nullptr;
	try{
		/* Create SQL statement */
		std::string sql = "SELECT rowid from FILES where PATH=?1 and HASH=?2";


		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_blob(stm, 1, path, wcslen(path)*sizeof(TCHAR), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, hash, strlen(hash), SQLITE_STATIC);
		rc = sqlite3_step(stm);
		sqlite_int64 rowid = sqlite3_column_int64(stm, 0);



		rc = sqlite3_blob_open(db, "main", "FILES", "DATI", rowid, 0, &BLOB);
		if (rc == 1){
			std::cout << sqlite3_errmsg(db) << std::endl;
			sendInt(client, 999);
			throw "invalid rowid";
		}
		else{ sendInt(client, 0); }

		int size = sqlite3_blob_bytes(BLOB);
		sendInt(client, size);
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int tot = 0;
		while (tot < size){
			if (tot + recvbuflen < size){
				int read = sqlite3_blob_read(BLOB, recvbuf, recvbuflen, tot);
				sendNbytes(client, recvbuf, recvbuflen);
			}
			else{
				int read = sqlite3_blob_read(BLOB, recvbuf, size - tot, tot);
				sendNbytes(client, recvbuf, size - tot);
				tot += size - tot;
				break;
			}
			tot += DEFAULT_BUFLEN;
		}


	}
	catch (...){
		sqlite3_finalize(stm);
		sqlite3_blob_close(BLOB);
		sqlite3_close(db);
		throw "error during restore";
	}
	sqlite3_finalize(stm);
	sqlite3_blob_close(BLOB);
	sqlite3_close(db);

	sendInt(client, -50);
	if (recInt(client) != -50){
		printf("error detected in the restore\n");
		throw("error detected in the restore");
	}
	else{
		printf("restore successfully completed\n");
	}
	sendInt(client, -50);
}

void Register(SOCKET client,std::string& nome){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	int sizename = recInt(client);
	nome = std::string((char*)recNbytes(client, sizename, recvbuf));

	int sizepass = recInt(client);
	std::string pass = std::string((char*)recNbytes(client, sizepass, recvbuf));
	{
		std::lock_guard<std::mutex> LG(m);

		if (clients.size() > MAX_N_CLIENTS){
			sendInt(client, 999);
			throw "too many users";
		}
		for (std::list<std::string>::iterator it = clients.begin(); it != clients.end(); ++it){
			std::string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999);
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
		sendInt(client, 999);
		throw "User already registered ";
	}
	else{
		my_file.close();
		sqlite3 *db = CreateDatabase(nome.c_str());
		sqlite3_stmt* stm = NULL;
		try{
			int  rc;
			std::string sql = "INSERT INTO CREDENTIAL (USER,PASS) VALUES (?1, ?2);";

			
			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

			rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
			rc = sqlite3_bind_text(stm, 2, pass.c_str(), pass.size(), SQLITE_STATIC);

			rc = sqlite3_step(stm);
			
		}
		catch(...){
			sqlite3_finalize(stm);
			sqlite3_close(db);
			throw "error during registration";
		}
		sqlite3_finalize(stm);
		sqlite3_close(db);
	}

	sendInt(client, -30);
	return;
};

void Login(SOCKET client, std::string& nome){
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	int sizename = recInt(client);
	nome = std::string((char*)recNbytes(client, sizename, recvbuf));

	int sfida1 = rand();
	sendInt(client, sfida1);
	int sfida2 = recInt(client);
	int sizepass = recInt(client);
	std::string pass = std::string((char*)recNbytes(client, sizepass, recvbuf));

	{
		std::lock_guard<std::mutex> LG(m);
		
		if (clients.size() > MAX_N_CLIENTS){
			sendInt(client, 999);
			throw "too many users";
		}
		for (std::list<std::string>::iterator it = clients.begin(); it != clients.end(); ++it){
			std::string IT = *it;
			if (!IT.compare(nome)){
				sendInt(client, 999);
				throw "user already served";
			}
		}
		clients.push_back(nome);
		printf("\nSto servendo %s\n", nome.c_str());
	}
	std::string temp = nome;
	temp.append(".db");
	std::ifstream my_file(temp);
	if (my_file)
	{	
		my_file.close();
		sqlite3 *db = CreateDatabase(nome.c_str());
		sqlite3_stmt* stm=nullptr;
		try{
			std::string sql = "SELECT PASS FROM CREDENTIAL WHERE USER=?1";			
			int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
			rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
			rc = sqlite3_step(stm);
			std::string passD = std::string((char*)sqlite3_column_text(stm, 0));
			
			passD.append(std::to_string(sfida2));
			passD.append(std::to_string(sfida1));
			std::string hash = sha256(passD);

			
			if (hash.compare(pass) != 0){
				sendInt(client, 999);
				throw "wrong password ";
			}
			
		}
		catch (...){
			sqlite3_finalize(stm);
			sqlite3_close(db);
			throw "error during login";
		}
		sqlite3_finalize(stm);
		sqlite3_close(db);
	}
	else{
		sendInt(client, 999);
		throw "User not registered ";
	}
	sendInt(client, -40);
	
};

