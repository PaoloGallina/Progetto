
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
	
	SOCKET server = ConnectClient();
	
	//questa sarà passata
	wstring* cartella_origine = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
	std::list <Oggetto*> allthefiles;
	Folder a(cartella_origine, allthefiles);

	ObjtoTxt(server, allthefiles);
	
	system("pause");
	delete cartella_origine;
	PulisciLista(allthefiles);
	
	closesocket(server);
	WSACleanup();

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
	bool first = 1;
	stringstream c;
	wstringstream b;

	for (std::list<Oggetto*>::iterator it = a.begin(); it != a.end(); ++it){

		Oggetto IT = *it;
		if (first == 0){
			b.write(L"\n", sizeof(wchar_t));
			c.write("\n", sizeof(char));
		}
		else{ 
			first = 0; 
		}
		c.write(IT.GetHash().c_str(), IT.GetHash().size());
		b.write(IT.GetPath().c_str(), IT.GetPath().size()*sizeof(wchar_t));
		b.write(L"\n", sizeof(wchar_t));
		b.write(IT.GetName().c_str(), IT.GetName().size()*sizeof(wchar_t));
		b.write(L"\n", sizeof(wchar_t));
		b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size()*sizeof(wchar_t));

		}
	cout << c.rdbuf();
	invFile(client, (char*)c.str().c_str(), c.str().size());
	invFile(client, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	for (std::list<Oggetto*>::iterator it = a.begin(); it != a.end(); ++it){
		Oggetto IT = *it;
		DWORD  size = IT.GetSize();
		invFile(client, (char*)&size, sizeof(DWORD));
	}
}

