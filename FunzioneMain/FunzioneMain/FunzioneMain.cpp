
#include "stdafx.h"
#include "Sock.h"
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "Folder.h"
#include "Sha.h"

#define _CRTDBG_MAP_ALLOC
using namespace std;

void PulisciLista(std::list < Oggetto *>& a);
void sync(SOCKET client,wstring* cartella);
list<Oggetto*> GetLastConfig(SOCKET server);
void SerializeList(SOCKET server, list<Oggetto*> daser);
list<Oggetto*> FilesDaMandare(list<Oggetto*>newconfig, list<Oggetto*> lastconfig);
int Register(SOCKET server);
int Login(SOCKET server);
int _tmain(){
	
	SOCKET server = ConnectClient();
	int c;

	char*a = (char*)malloc(50);
	gets(a);
		if (strcmp(a,"login")==0){ 
			c = Login(server); }
		else{ 
			c = Register(server); }

	if (c==999){
		closeConn(server);
		WSACleanup();
		return 0;
	}
	while (1){

		//effettuo una sync
		wstring *cartella = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
		sync(server, cartella);
		delete cartella;
		this_thread::sleep_for(chrono::seconds(10));
		system("cls");
	}

	closeConn(server);
	WSACleanup();

	_CrtDumpMemoryLeaks();
	return 0;
}

void sync(SOCKET server,wstring* cartella){
	
	std::list <Oggetto*> newconfig, lastconfig, missingfiles;

	Folder cartelle(cartella, newconfig);

	printf("\nI get from the server the last configuration\n");
	lastconfig = GetLastConfig(server);
	missingfiles = FilesDaMandare(newconfig, lastconfig);

	printf("I ask to the server to do 10 SYNC\n");
	sendInt(server, 10);//the required operation is a sync
	printf("\nI send to the server the new configuration\n");
	SerializeList(server, newconfig);
	printf("\nI send to the server the list of the missing files\n");
	SerializeList(server, missingfiles);

	//questa parte sarebbe carino metterla in una funzione a se
	printf("\n\nI start sending to the server the file that are needed\n");
	DWORD read;
	int tot;
	int lunghezza = recInt(server);
	while (lunghezza != -10){
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		wstring wpath((wchar_t*)recNbytes(server, lunghezza, recvbuf));

		HANDLE file;
		
		for (std::list<Oggetto*>::iterator it = newconfig.begin(); it != newconfig.end(); ++it){
			Oggetto IT = *it;
			if (!IT.GetPath().compare(wpath)){
				file = IT.GetHandle();
				break;
			}
			
		}
		tot = 0;
		while (1){
			ReadFile(file, recvbuf, recvbuflen, &read, NULL);
			if (read != recvbuflen){
				sendNbytes(server, recvbuf, read);
				tot += read;
				break;
			}
			tot += read;
			sendNbytes(server, recvbuf, read);
		}
		lunghezza = recInt(server);
		SetFilePointer(file, 0, 0, 0);
	}
	sendInt(server, -10);
	if (recInt(server) == -10){
		printf("Sync terminated, no more file needed");
	}
	else{
		printf("ERROR during sync termination");
	}

	//pulizia
	
	PulisciLista(newconfig);
	PulisciLista(lastconfig);
	PulisciLista(missingfiles);
}

list<Oggetto*> GetLastConfig(SOCKET server){
	printf("I send the #20 requested last config");
	sendInt(server, 20);
	list<Oggetto*>last;
	printf("I get the number of files in the last config\n");
	int n = recInt(server);

	char* Hash = recvFile(server);
	wchar_t* PathNameLast = (wchar_t*)recvFile(server);
	istringstream fhash(Hash);
	wistringstream fpath(PathNameLast);


	int t = 0;
	while (t<n){
		wchar_t buf1[512];
		char buf4[512];
		fpath.getline(buf1, 512);
		fhash.getline(buf4, 512);
		last.push_front(new Oggetto(buf1, L"", L"", buf4, 0, INVALID_HANDLE_VALUE));
		t++;
	}

	::free(Hash);
	::free(PathNameLast);

	if (recInt(server)!=-20){
		printf("An error occurred while receiving last config");
	}
	return last;
}

void SerializeList(SOCKET server,list<Oggetto*> daser){
	//serialize list of files
	stringstream c;
	wstringstream b;

	for (std::list<Oggetto*>::iterator it = daser.begin(); it != daser.end(); ++it){

		Oggetto IT = *it;

		c.write(IT.GetHash().c_str(), IT.GetHash().size());
		c.write("\n", 1);

		b.write(IT.GetPath().c_str(), IT.GetPath().size());
		b.write(L"\n", 1);
		b.write(IT.GetName().c_str(), IT.GetName().size());
		b.write(L"\n", 1);
		b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size());
		b.write(L"\n", 1);
	}
	printf("I send the number of obj in the list\n");
	sendInt(server, daser.size());

	printf("I send the size of first file and the file\n");
	sendInt(server, c.str().size());
	invFile(server, (char*)c.str().c_str(), c.str().size());

	printf("I send the size of second fileand the file\n");
	sendInt(server, b.str().size()*sizeof(wchar_t));
	invFile(server, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	for (std::list<Oggetto*>::iterator it = daser.begin(); it != daser.end(); ++it){
		Oggetto IT = *it;
		DWORD  size = IT.GetSize();
		invFile(server, (char*)&size, sizeof(DWORD));
	}
	return;
}

list<Oggetto*> FilesDaMandare(list<Oggetto*>newconfig, list<Oggetto*> lastconfig){
	
	list<Oggetto*> da_mandare;
	
	for (std::list<Oggetto*>::iterator it = newconfig.begin(); it != newconfig.end(); ++it){
		Oggetto IT = *it;
		bool is_on_the_server = false;
		for (std::list<Oggetto*>::iterator it2 = lastconfig.begin(); it2 != lastconfig.end(); ++it2){
			Oggetto IT2 = *it2;
			if (!IT.GetPath().compare(IT2.GetPath()) && !IT.GetHash().compare(IT2.GetHash())){
				is_on_the_server = true;
			}
		}
		if (is_on_the_server == false){
			da_mandare.push_front(new Oggetto(IT.GetPath(), IT.GetName(), IT.GetLastModified(),IT.GetHash(), IT.GetSize(), INVALID_HANDLE_VALUE));
		}
	}
	return da_mandare;
}

int Register(SOCKET server){
	sendInt(server, 30);
	char nome[50];
	gets(nome);
	sendInt(server, strlen(nome)+1);
	sendNbytes(server, nome, strlen(nome)+1);
	char pass[50];
	gets(pass);
	sendInt(server, strlen(pass)+1);
	sendNbytes(server, pass, strlen(pass)+1);
	return recInt(server);
}

int Login(SOCKET server){
	sendInt(server, 40);
	char nome[50];
	gets(nome);
	sendInt(server, strlen(nome)+1);
	sendNbytes(server, nome, strlen(nome)+1);
	char pass[50];
	gets(pass);
	sendInt(server, strlen(pass)+1);
	sendNbytes(server, pass, strlen(pass)+1);
	return recInt(server);
}

void PulisciLista(std::list < Oggetto *>& a){
	Oggetto* p2;
	while (!a.empty()){
		p2 = a.front();
		if (p2->GetHandle() != INVALID_HANDLE_VALUE){
			if (!CloseHandle(p2->GetHandle())){
				std::cout << "HO CHIUSO UNA HANDLE MALE" << endl;
				std::cout << GetLastError() << endl;
			}
		}

		a.pop_front();
		delete p2;
		p2 = NULL;
	}
};
