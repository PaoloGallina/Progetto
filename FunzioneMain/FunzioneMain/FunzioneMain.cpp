
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
void sync(SOCKET client);

int _tmain(){
		{
	SOCKET server = ConnectClient();
	while (1){
		


		//effettuo una sync
		sync(server);



		system("pause");
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

void sync(SOCKET server){
	//send Op richiesta
	sendInt(server, 10);
	
	//questa sarà passata dall'utente
	wstring* cartella_origine = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
	std::list <Oggetto*> allthefiles;
	//guarda il set last error 0
	Folder cartelle(cartella_origine, allthefiles);



	//serialize list of files
	stringstream c;
	wstringstream b;
	
	for (std::list<Oggetto*>::iterator it = allthefiles.begin(); it != allthefiles.end(); ++it){

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
	sendInt(server, allthefiles.size());
	
	printf("I send the size of first file\n");
	sendInt(server, c.str().size());
	invFile(server, (char*)c.str().c_str(), c.str().size());

	printf("I send the size of second file\n");
	sendInt(server, b.str().size()*sizeof(wchar_t));
	invFile(server, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	for (std::list<Oggetto*>::iterator it = allthefiles.begin(); it != allthefiles.end(); ++it){
		Oggetto IT = *it;
		DWORD  size = IT.GetSize();
		invFile(server, (char*)&size, sizeof(DWORD));
	}

	//send requested files
	int lunghezza = recInt(server);
	while (lunghezza != -10){
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		wstring wpath((wchar_t*)recNbytes(server, lunghezza, recvbuf));

		std::ifstream A(wpath, std::ios_base::binary);
		
		while (1){
			A.read(recvbuf, recvbuflen);
			if (A.eof()){
				sendNbytes(server, recvbuf, A.gcount());
				break;
			}
			sendNbytes(server, recvbuf, A.gcount());
		}
		lunghezza = recInt(server);

		A.close();
	}
	

	//pulizia
	delete cartella_origine;
	PulisciLista(allthefiles);
}

