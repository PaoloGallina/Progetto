
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

#define _CRTDBG_MAP_ALLOC
using namespace std;

int _tmain(){

	//deve essere passata in qualche modo,
	//Può essere più di una??
	wstring* cartella_origine = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
	wstring* file_output = new wstring(L"out.txt");
	std::list <Oggetto*> allthefiles;
	std::wofstream f(*file_output, std::wofstream::out);
	

//	while (1)
	{
		Folder a(cartella_origine, file_output, f, allthefiles);
		
		cout << "SLEEEP";
//		std::this_thread::sleep_for(chrono::minutes(1));
	}
	f.close();
	
	system("pause");
	delete cartella_origine;
	delete file_output;
	_CrtDumpMemoryLeaks();
	return 0;
}

