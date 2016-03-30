#include "stdafx.h"
#include "fstream"
#include <string>
#include <thread>
#include <string.h>
#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <future>
#include <list>
#include <chrono>
#include <strsafe.h>
#include "Cartella.h"
#include <windows.h>


using namespace std;




Cartella::Cartella(std::wstring* cartella_origine, std::list <Oggetto*>& allthefiles, HANDLE hpipe, int flag)
{
	SetLastError(0);
	WIN32_FIND_DATA find_file_data;
	HANDLE Ffile = FindFirstFileW((*cartella_origine + L"\\*").c_str(), &find_file_data);

	if (Ffile == INVALID_HANDLE_VALUE){
		printf("Non ho trovato nulla, la cartella esiste?!\n");
		throw "La cartella selezionata non esiste";
	}
	DWORD max_size = 200000000;
	for (bool i; GetLastError() != ERROR_NO_MORE_FILES; i = FindNextFile(Ffile, &find_file_data)){
		if (find_file_data.nFileSizeHigh == 0 && find_file_data.nFileSizeLow < max_size){
			wstring filepath = *cartella_origine + L"\\" + find_file_data.cFileName;
			if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(find_file_data.cFileName, L"..") != 0 && wcscmp(find_file_data.cFileName, L".") != 0){
				this->contains.push_front(new Cartella(&filepath, allthefiles, hpipe, flag));
				SetLastError(0);
			}
			else if (!(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				wstring desk(L"desktop.ini");
				if (desk.compare(find_file_data.cFileName)){
					HANDLE handle = CreateFileW(filepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
					#pragma region "Gestione file aperto in altro processo"
					int t = 6;
					while (handle == INVALID_HANDLE_VALUE && GetLastError() != ERROR_FILE_NOT_FOUND){
						printf("\nnon posso aprire un della cartella file\n");
						this_thread::sleep_for(chrono::seconds(2));
						handle = CreateFileW(filepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
						t--;
						if (t < 0){
							DWORD NuByRe;
							WriteFile(hpipe, L"ACCESS ", 7 * sizeof(wchar_t), &NuByRe, NULL);
							printf("\na file cannot be accessed\n i read the buffer wrote to the clientc# and thrown an exception");
							throw "a file cannot be accessed";
						}
					}
					#pragma endregion
					TCHAR *lpszString = (TCHAR*)malloc(50 * sizeof(TCHAR));
					try{
						#pragma region "creando il tag dell'ultima modifica"
						FILETIME ultimamodifica;
						GetFileTime(handle, NULL, NULL, &ultimamodifica);
						SYSTEMTIME struct_ultima_modifica, stLocal;
						FileTimeToSystemTime(&ultimamodifica, &struct_ultima_modifica);
						// Convert the last-write time to local time.
						FileTimeToSystemTime(&ultimamodifica, &struct_ultima_modifica);
						SystemTimeToTzSpecificLocalTime(NULL, &struct_ultima_modifica, &stLocal);
						// Build a string showing the date and time.
						StringCchPrintf(lpszString, 50,TEXT("%02d/%02d/%d  %02d:%02d:%02d"),stLocal.wDay, stLocal.wMonth, stLocal.wYear,stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
						wstring lastmodified(lpszString);
						#pragma endregion
						if (handle != INVALID_HANDLE_VALUE){
							allthefiles.push_front(new Oggetto(filepath, find_file_data.cFileName, lastmodified, GetFileSize(handle, NULL), handle));
						}
					}
					catch (...){
						free(lpszString);
						throw "errore generico";
					}
					free(lpszString);
				}
			}
		}
	}

	FindClose(Ffile); 
}

Cartella::~Cartella()
{
	Cartella* p;
	while (!this->contains.empty()){
		p = this->contains.front();
		this->contains.pop_front();	
		delete p;
		p = NULL;
	}
}




