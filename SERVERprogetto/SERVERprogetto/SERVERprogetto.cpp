

#include "stdafx.h"
#include "Sock.h"
#include "Datab.h"
#include "Oggetto.h"
#include "Folder.h"
#include <iostream>
#include <list>
#include <fcntl.h>
#include <io.h>


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
		listaobj.push_front(new Oggetto(buf1, buf2, buf3, buf4,*((DWORD *) recNbytes(client,sizeof(DWORD),buf5,512))));
		t++;
	}

	free(Hash);
	free(PathNameLast);
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
	system("pause");
	system("cls");

}