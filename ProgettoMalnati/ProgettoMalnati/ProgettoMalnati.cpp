// LABORATORIO3_FGREP.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "fstream"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Folder.h"


#define _CRTDBG_MAP_ALLOC

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{	
	cout << "ciao"<<endl;
	try{
		cout << "a";
		cout << "b";
		cout << "c";
		throw "err";

	}
	catch (...){
		
		cout << "asdadas";
	}
	

	{
		
		if (argc != 4){
			wcout << L"Impara ad usare questo tool!\n" << endl;
			return 1;
		}
		wstring* cartella_origine = new wstring(argv[1]);
		wstring* stringa = new wstring(argv[2]);
		wstring* file_output = new wstring(argv[3]);
		if (stringa->size() > 50 || stringa->size() < 2 || cartella_origine->size() > 101 || cartella_origine->size() < 4 || file_output->size() > 101 || file_output->size() < 4){
			wcout << L"Impara ad usare questo tool!\n" << *cartella_origine << endl;
			return 1;
		}
		std::wofstream f(*file_output, std::wofstream::out);
		
		Folder a(cartella_origine, file_output, f, *cartella_origine);
		



		system("pause");
		system("cls");

		shared_future<wstring> p;
		while (!Folder::lista_promesse.empty()){
			p = Folder::lista_promesse.front();
			std::wcout << L"THREAD COMPLETATO"+p.get() << std::endl;
			Folder::lista_promesse.pop_front();
		}

		f.close();
		delete cartella_origine;
		delete stringa;
		delete file_output;
	}

	system("pause");


	_CrtDumpMemoryLeaks();
	return 0;
}

