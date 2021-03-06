#include "stdafx.h"
#include <thread>
#include <string>
#include <chrono>
#include "Sock.h"

int _tmain(int argc, _TCHAR* argv[])
{		

	std::thread uno(server);

	uno.join();
	
	return 0;
}

int hearPIPES(){
	LPTSTR a = TEXT("\\\\.\\pipe\\pipename");
	CHAR* buffer = (CHAR*)malloc(27000000 * sizeof(CHAR));

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

		BOOL i = ReadFile(hpipe, buffer, (27000000 * sizeof(CHAR)), &NuByRe, NULL);

		if (i){
			_tprintf(TEXT("SERVER::I SUCCEED IN READING: %d byte\n"), NuByRe);
		}
		else{
			_tprintf(TEXT("SERVER::NOT ABLE TO READ %d\n"), GetLastError());

		}

		std::string mess1(buffer, NuByRe);

		std::ofstream ofs("C:\\Users\\Paolo\\Desktop\\PROVA2\\video.copia2.wmv", std::ios::binary);
		ofs << mess1;
		ofs.close();



		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
	return 0;
}
