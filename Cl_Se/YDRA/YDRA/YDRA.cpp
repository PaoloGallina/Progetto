// YDRA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	while (1){
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		LPTSTR a = _tcsdup(TEXT("C:\\Windows\\notepad"));
		BOOL i = CreateProcess(NULL, a, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		WaitForSingleObject(pi.hProcess, 5000);
	}
	return 0;
}

