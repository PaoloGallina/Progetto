// Cl_Se=SERVER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>

using namespace std;

int hearPIPES(){
  //creata nel processo SERVER
	return 0;
}

int talkPIPES(){
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	DWORD NuByRe;

	
	HANDLE hpipe = CreateFile(a, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hpipe == INVALID_HANDLE_VALUE){
		while (hpipe == INVALID_HANDLE_VALUE){
			_tprintf(TEXT("CLIENT ERROR::%d\n"), GetLastError());
			this_thread::sleep_for(chrono::milliseconds(2000));
			hpipe = CreateFile(a, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		}
	}
	_tprintf(TEXT("The connention with the other process is working"));

	
	
	std::ostringstream sstringa;
	std::ifstream ifs("C:\\Users\\Paolo\\Desktop\\PROVA2\\VIDEO.wmv", std::ios::binary);
	sstringa << ifs.rdbuf();
	cout << GetLastError();

	string mess1(sstringa.str());


	ifs.close();
	std::ofstream ofs("C:\\Users\\Paolo\\Desktop\\PROVA2\\lavoltabuona.jpg", std::ios::binary);
	ofs << mess1;
	ofs.close();


	while (1){
		BOOL i = WriteFile(hpipe, (mess1.c_str()), mess1.size(), &NuByRe, NULL);
		
		if (i=0){
			_tprintf(TEXT("CLIENT ERROR::%d\n"), GetLastError());
		}
	}
	return 0;
}

int talk(){ 
	
	
	return 0; }

int _tmain(int argc, _TCHAR* argv[])
{

	//thread uno(hear);
	thread due(talk);

	due.join();
	//uno.join();

	system("pause");
	return 0;
}