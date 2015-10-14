
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
void ObjtoTxt(SOCKET client, std::list < Oggetto *>& a);

int _tmain(){
		{
	SOCKET server = ConnectClient();
	while (1){
		//questa sarà passata dall'utente
		wstring* cartella_origine = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
		std::list <Oggetto*> allthefiles;
		//guarda il set last error 0
		Folder a(cartella_origine, allthefiles);

		sendInt(server, 10);
		ObjtoTxt(server, allthefiles);

		system("pause");
		delete cartella_origine;
		PulisciLista(allthefiles);
	}

	closesocket(server);
	WSACleanup();
}
	_CrtDumpMemoryLeaks();
	return 0;
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

void ObjtoTxt(SOCKET client,std::list < Oggetto *>& a){
	
	stringstream c;
	wstringstream b;

	for (std::list<Oggetto*>::iterator it = a.begin(); it != a.end(); ++it){

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
	sendInt(client, a.size());
	
	printf("I send the size of first file\n");
	sendInt(client, c.str().size());
	invFile(client, (char*)c.str().c_str(), c.str().size());

	printf("I send the size of second file\n");
	sendInt(client, b.str().size()*sizeof(wchar_t));
	invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	for (std::list<Oggetto*>::iterator it = a.begin(); it != a.end(); ++it){
		Oggetto IT = *it;
		DWORD  size = IT.GetSize();
		invFile(client, (char*)&size, sizeof(DWORD));
	}
}

