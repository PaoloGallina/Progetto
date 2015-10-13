
#include "stdafx.h"
#include "Datab.h"
#include "Oggetto.h"
#include "Folder.h"
#include <iostream>
#include <windows.h>
#include <iostream>
#include <stream>
#include <list>
#include <fcntl.h>
#include <io.h>
#include "Sock.h"

#define _CRTDBG_MAP_ALLOC
using namespace std;

void TxtToList(SOCKET, list < Oggetto *>&);
void PulisciLista(std::list < Oggetto *>&);
void Sync(SOCKET client, std::string nome);

int _tmain(int argc, _TCHAR* argv[])
{
		//autenticazione+più di un client+devoritornare anche il nome utente
		SOCKET client= ConnectServer();
		string nome = "stub";
		while (1){
			int op = opRichiesta(client);
			if (op == 10){
				Sync(client, nome);
			}
			else if (op == 999){
				break;
			}
		}
		closesocket(client);
		WSACleanup();

	_CrtDumpMemoryLeaks();
	return 0;
}

void TxtToList(SOCKET client,list < Oggetto *>& listaobj){

	wchar_t* path =(wchar_t*) recvFile(client);
	char* hash = recvFile(client);
	char* size = recvFile(client);
	char* name = recvFile(client);
	wchar_t* lastmodified = (wchar_t*)recvFile(client);
	wistringstream a(path);
	wistringstream b(lastmodified);
	istringstream c(size);//forse è meglio passare un array di double
	istringstream d(hash);
	istringstream e(name);


	while (1){
		
		
	}
	
	return;
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
		nuovaVersione(db, listaobj, da_chiedere);
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
	system("cls");

}