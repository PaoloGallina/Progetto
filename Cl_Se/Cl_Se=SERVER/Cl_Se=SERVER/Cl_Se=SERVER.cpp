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
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	TCHAR* buffer = (TCHAR*)malloc(512 * sizeof(TCHAR));
	
	if (buffer == nullptr){
		_tprintf(TEXT("SERVER::%d\n"), GetLastError());
	}
	else{
		_tprintf(TEXT("SERVER::The buffer has been created\n"));
	}
	DWORD NuByRe;
	HANDLE hpipe = CreateNamedPipe(a, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 500, NULL);
	
	if (hpipe == INVALID_HANDLE_VALUE){
		_tprintf(TEXT("SERVER:: ERROR NUMBER: %d\n"), GetLastError());
	}
	else{
		_tprintf(TEXT("SERVER::The pipe has been created\n"));
	}
	ConnectNamedPipe(hpipe, nullptr);
	
	while (1){

		BOOL i = ReadFile(hpipe, buffer, (512 * sizeof(TCHAR)), &NuByRe, NULL);
		

		if (i){
			_tprintf(TEXT("SERVER::I SUCCEED IN READING: %d bytes\n THE MESSAGE IS %s\n"), NuByRe, buffer);
		}
		else{
			_tprintf(TEXT("SERVER::NOT ABLE TO READ %d\n"), GetLastError());

		}

		this_thread::sleep_for(chrono::milliseconds(2000));
	}
	return 0;
}

int talk(){
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{		

	thread uno(hear);
//	thread due(talk);

//	due.join();
	uno.join();
	
	system("pause");
	return 0;
}