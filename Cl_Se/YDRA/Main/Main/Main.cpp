

#include "stdafx.h"
#include <windows.h>
#include <iostream>



	int _tmain(int argc, _TCHAR* argv[]){
		while (1){
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			LPTSTR a = _tcsdup(TEXT(".\\YDRA"));
			BOOL i = CreateProcess(NULL, a, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
			std::cout << "GIULIA, DIVERTITI";
			WaitForSingleObject(pi.hProcess, 5000);
		}
		return 0;
	}

