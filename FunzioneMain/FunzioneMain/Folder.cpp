#include "stdafx.h"
#include "Folder.h"
#include "fstream"
#include <windows.h>
#include <string>
#include <string.h>
#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <future>
#include <list>
#include <chrono>

using namespace std;

list<shared_future<wstring>> Folder::lista_promesse; //Devo definire la variabile statica!

wstring FIND(wstring a){
	
	std::this_thread::sleep_for(chrono::milliseconds(1));
	return a;
}

Folder::Folder(std::wstring* cartella_origine, std::wstring* file_output,std::wofstream& f,std::wstring nome)
{
	BOOL i;
	this->name = nome;

	WIN32_FIND_DATA find_file_data;
	HANDLE Ffile = FindFirstFile(cartella_origine->c_str(), &find_file_data);
	cartella_origine->pop_back();
	cartella_origine->pop_back();

	if (Ffile == INVALID_HANDLE_VALUE){
		std::wcout << L"Non ho trovato nulla, la cartella esiste?!\n" << *cartella_origine << std::endl;
		f << L"Non ho trovato nulla, la cartella esiste?!\n" << *cartella_origine << std::endl;
		//lancia una eccezione
	}

	for (; GetLastError() != ERROR_NO_MORE_FILES; i = FindNextFile(Ffile, &find_file_data)){
		std::wcout << *cartella_origine + L"\\" + find_file_data.cFileName + L"----->" + std::to_wstring(find_file_data.dwFileAttributes) << std::endl;
		f << *cartella_origine + L"\\" + find_file_data.cFileName + L"----->" + std::to_wstring(find_file_data.dwFileAttributes) << std::endl;

		if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(find_file_data.cFileName, L"..") != 0 && wcscmp(find_file_data.cFileName, L".") != 0){
			std::wstring nuova = *cartella_origine + L"\\" + find_file_data.cFileName + L"\\*";
			std::wcout <<L"NUOVA CARTELLA TROVATA!! " +nuova<<std::endl;
			this->contains.push_front(new Folder(&nuova, file_output, f, nuova));
			SetLastError(0);
		}
		else if (! (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
			std::future<wstring> future1 = std::async(FIND, *cartella_origine + L"\\" + find_file_data.cFileName);//se chiudessi subito la graffa il thread sarebbe creato e subito dopo distrutto!!
			std::shared_future<wstring> promise1 = future1.share();
			lista_promesse.push_front(promise1);

		}

	}
	
	FindClose(Ffile); //ricordati di chiudere sempre le HANDLE
}

//Visto che creiamo il distruttore specifico è ovvio che abbiamo bisogno del costruttore di copia e assegnazione
Folder::~Folder()
{
	Folder* p;
	while (!this->contains.empty()){
		p = this->contains.front();
		this->contains.pop_front();
		std::wcout << L"deleting " + p->name<<std::endl;
		delete p;
		p = NULL;
	}
}




