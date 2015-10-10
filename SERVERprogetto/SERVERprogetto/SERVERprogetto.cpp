
#include "stdafx.h"
#include "Datab.h"
#include "Oggetto.h"
#include "Folder.h"
#include <iostream>
#include <windows.h>
#include <iostream>
#include <list>
#include <fcntl.h>
#include <io.h>

#define _CRTDBG_MAP_ALLOC
using namespace std;

void TxtToList(string, list < Oggetto *>&);
void PulisciLista(std::list < Oggetto *>&);

int _tmain(int argc, _TCHAR* argv[])
{	
	//MANCA LA PARTE DELLA COMUNICAZIONE DELLA GESTIONE DEGLI ERRORI; UN PO' DI TESTING E L'INTERFACCIA GRAFICA
	
	while (1){
		sqlite3 *db = CreateDatabase();
		std::list < Oggetto *> listaobj;
	
		TxtToList("", listaobj); // è solo uno stub, dovrebbe connettersi con il client e chiedere la lista dei file attuali, invece li prende dal filesystem
		std::list < Oggetto *> da_chiedere = CheFILEvoglio(db, listaobj);
		if (da_chiedere.size() != 0 || file_cancellati(db, listaobj.size()) != 0){
				nuovaVersione(db, listaobj, da_chiedere);
		}
		else{
			wcout << L"\nThe database is updated\n" << endl;
		}
		for (int i=max(1, GetUltimaVersione(db)-2); i <= GetUltimaVersione(db); i++){
			ReadVERSIONE(db, i);
		}

		ReadFILES(db);
		PulisciLista(listaobj);
		PulisciLista(da_chiedere);
		sqlite3_close(db); 
		this_thread::sleep_for(chrono::seconds(60));
		system("cls");
	}

	_CrtDumpMemoryLeaks();
	
	return 0;
}

void TxtToList(string stub, list < Oggetto *>& listaobj){
	//This function that now is just a stub will transform the txt file passed by the second process into the corresponding listobj 
	//stub now is useless it will be the string containing the txt file (notice that we do not have access to Folder functions here!... )
	//Serve per comunicare

	wstring *a = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
	Folder cartelle(a, listaobj);
	delete a;

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

