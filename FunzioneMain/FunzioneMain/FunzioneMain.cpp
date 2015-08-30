
#include "stdafx.h"
#include <fstream>
#include <windows.h>
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

int _tmain(){

	wstring* cartella_origine = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
	std::list <Oggetto*> allthefiles;


//	while (1)
	{
		allthefiles.clear(); //NON BASTA, DEVI ANCHE CANCELLARE GLI OGGETTI DELLA LISTA!---> rischi un memory leak, così facendo distruggi solo i pointers
		Folder a(cartella_origine, allthefiles);
		
		cout << "SLEEEP";
//		std::this_thread::sleep_for(chrono::minutes(1));
	}
	
	system("pause");
	delete cartella_origine;
	_CrtDumpMemoryLeaks();
	return 0;
}

