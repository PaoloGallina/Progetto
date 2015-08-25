// Cl_Se=SERVER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <Windows.h>

using namespace std;

int hear(){
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	TCHAR* buffer = (TCHAR*)malloc(512 * sizeof(TCHAR));
	DWORD NuByRe;
	HANDLE hpipe = CreateNamedPipe(a, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 500, NULL);
	if (hpipe == INVALID_HANDLE_VALUE){
		_tprintf(TEXT("SERVER::%d\n"), GetLastError());
	}
	BOOL i = ReadFile(hpipe, buffer, (512 * sizeof(TCHAR)), &NuByRe, NULL);
	if (!i)
	_tprintf(TEXT("Ciao %d \n %s"),NuByRe, buffer);
	this_thread::sleep_for(chrono::milliseconds(2000));
	return 0;
}

int talk(){
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	TCHAR* buffer = (TCHAR*)malloc(512 * sizeof(TCHAR));
	DWORD NuByRe;
	LPTSTR mess = TEXT("DEFAULT MESSAGE");
	HANDLE hpipe = CreateFile(a,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

	if (hpipe == INVALID_HANDLE_VALUE){
		_tprintf(TEXT("%d\n"), GetLastError());
	}

	BOOL i = WriteFile(hpipe, mess, sizeof(mess), &NuByRe, NULL);
	_tprintf(TEXT("Sono riuscito  a scrivere qualcosa di %d Bytes , %d\n"),NuByRe,GetLastError());

	this_thread::sleep_for(chrono::milliseconds(2000));
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{		
	thread uno(talk);
	thread due(hear);


	this_thread::sleep_for(chrono::milliseconds(2000));
	due.join();
	uno.join();
	
	system("pause");
	return 0;
}



