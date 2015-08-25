// Cl_Se=SERVER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <string>
#include <chrono>
#include <iostream>
#include <Windows.h>

using namespace std;

int hear(){
  //creata nel processo SERVER
	return 0;
}

int talk(){
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	TCHAR* buffer = (TCHAR*)malloc(512 * sizeof(TCHAR));
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
	LPTSTR mess = TEXT("DEFAULT MESSAGE");
	wstring mess1;
	while (1){
		wcin >> mess1;
		BOOL i = WriteFile(hpipe, (mess1.c_str()), (lstrlen((mess1.c_str())) + 1)*sizeof(TCHAR), &NuByRe, NULL);
		if (i=0){
			_tprintf(TEXT("CLIENT ERROR::%d\n"), GetLastError());
		}
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	//thread uno(hear);
	thread due(talk);

	due.join();
	//uno.join();

	system("pause");
	return 0;
}