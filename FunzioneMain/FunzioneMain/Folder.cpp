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
#include "Folder.h"
#include <windows.h>

using namespace std;


Folder::Folder(std::wstring* cartella_origine, std::list <Oggetto*>& allthefiles)
{
	SetLastError(0);
	wstring desk(L"desktop.ini");
	this->name = *cartella_origine;
	WIN32_FIND_DATA find_file_data;
	HANDLE Ffile = FindFirstFileW((*cartella_origine+L"\\*").c_str(), &find_file_data);		
	
	if (Ffile == INVALID_HANDLE_VALUE){
		std::wcout << L"Non ho trovato nulla, la cartella esiste?!\n" << *cartella_origine << std::endl;
		
	}

	for (bool i; GetLastError() != ERROR_NO_MORE_FILES;i=FindNextFile(Ffile, &find_file_data)){
			
		wstring filepath = *cartella_origine + L"\\" + find_file_data.cFileName;
		
		if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(find_file_data.cFileName, L"..") != 0 && wcscmp(find_file_data.cFileName, L".") != 0){
				
			this->contains.push_front(new Folder(&filepath, allthefiles));
			SetLastError(0);
		}
		else if (! (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
			if (desk.compare(find_file_data.cFileName)){
			
				HANDLE handle = CreateFileW(filepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
				while (handle == INVALID_HANDLE_VALUE&&GetLastError()!=ERROR_FILE_NOT_FOUND){
					wcout << "non posso aprire il file " << filepath << endl;
					this_thread::sleep_for(chrono::seconds(10));
					handle = CreateFileW(filepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
				}
				FILETIME ultimamodifica;
				GetFileTime(handle, NULL, NULL, &ultimamodifica);

				SYSTEMTIME struct_ultima_modifica, stLocal;
				FileTimeToSystemTime(&ultimamodifica, &struct_ultima_modifica);
				TCHAR *lpszString = (TCHAR*)malloc(50 * sizeof(TCHAR));
				// Convert the last-write time to local time.
				FileTimeToSystemTime(&ultimamodifica, &struct_ultima_modifica);
				SystemTimeToTzSpecificLocalTime(NULL, &struct_ultima_modifica, &stLocal);
				// Build a string showing the date and time.
				StringCchPrintf(lpszString, 50,
					TEXT("%02d %02d %d  %02d %02d"),
					stLocal.wMonth, stLocal.wDay, stLocal.wYear,
					stLocal.wHour, stLocal.wMinute);

				wstring lastmodified(lpszString);
				free(lpszString);

				if (handle != INVALID_HANDLE_VALUE){
					allthefiles.push_front(new Oggetto(filepath, find_file_data.cFileName, lastmodified, GetFileSize(handle, NULL), handle));
				}
			}
		}

	}
	
	FindClose(Ffile); //ricordati di chiudere sempre le HANDLE
}

Folder::~Folder()
{
	Folder* p;
	while (!this->contains.empty()){
		p = this->contains.front();
		this->contains.pop_front();
		delete p;
		p = NULL;
	}
}




