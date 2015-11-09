
#include "stdafx.h"
#include "Sock.h"
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "Cartella.h"
#include "Sha.h"
#include <Shlobj.h>
#include <Shlwapi.h>

#define _CRTDBG_MAP_ALLOC
using namespace std;

HANDLE hpipe;

void PulisciLista(std::list < Oggetto *>& a);
void sync(SOCKET client,wstring* cartella);
list<Oggetto*> GetLastConfig(SOCKET server);
void SerializeList(SOCKET server, list<Oggetto*> daser);
list<Oggetto*> FilesDaMandare(list<Oggetto*>newconfig, list<Oggetto*> lastconfig);
void Restore(SOCKET server, wstring wpath, string hash);
int Register(SOCKET server);
int Login(SOCKET server);
void Talk();

int _tmain(){
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int c=999;
	DWORD NuByRe;

	//ERRORRRRREEE IL NOME NON DOVREBBE ESSERE FISSO
	hpipe = CreateFile( TEXT("\\\\.\\pipe\\myNamedPipe1"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hpipe == INVALID_HANDLE_VALUE){
		while (hpipe == INVALID_HANDLE_VALUE){
			_tprintf(TEXT("INVALID CLIENT ERROR::%d\n"), GetLastError());
			this_thread::sleep_for(chrono::milliseconds(2000));
			hpipe = CreateFile(TEXT("\\\\.\\pipe\\myNamedPipe1"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		}
	}

	SOCKET server=INVALID_SOCKET;

	while (c == 999){
		server = ConnectClient();

		int size;
		ReadFile(hpipe, recvbuf, 4, &NuByRe, NULL);
		size = *((int*)recvbuf);
		
		ReadFile(hpipe, recvbuf, 1, &NuByRe, NULL);
		ReadFile(hpipe, recvbuf, size, &NuByRe, NULL);
		recvbuf[NuByRe] = '\0';
		

		if (strcmp(recvbuf, "login") == 0){
			c = Login(server);
		}
		else if(strcmp(recvbuf, "register")==0){
			c = Register(server);
		}
		else{
			closeConn(server);
			return -1;
		}
		if (c == 999){
			WriteFile(hpipe, L"errore\n", 7 * sizeof(wchar_t), &NuByRe, NULL);		
		}
		else{
			WriteFile(hpipe, L"OK\n", 7 * sizeof(wchar_t), &NuByRe, NULL);

		}
		if (c == 999){
			closeConn(server);
		}
	}



	while (1){

		//effettuo una sync
		wstring *cartella = new wstring(L"C:\\Users\\Paolo\\Desktop\\PROVA2");
		sync(server, cartella);
		this_thread::sleep_for(chrono::seconds(10));
		delete cartella;
		
		auto debug =GetLastConfig(server);
		Restore(server, debug.back()->GetPath(), debug.back()->GetHash());
		
		system("cls");
	}

	closeConn(server);
	WSACleanup();

	_CrtDumpMemoryLeaks();
	return 0;
}

void sync(SOCKET server,wstring* cartella){
	
	std::list <Oggetto*> newconfig, lastconfig, missingfiles;

	Cartella cartelle(cartella, newconfig);

	printf("\nI get from the server the last configuration\n");
	lastconfig = GetLastConfig(server);
	missingfiles = FilesDaMandare(newconfig, lastconfig);

	printf("I ask to the server to do 10 SYNC\n");
	sendInt(server, 10);//the required operation is a sync
	printf("\nI send to the server the new configuration\n");
	SerializeList(server, newconfig);
	printf("\nI send to the server the list of the missing files\n");
	SerializeList(server, missingfiles);

	//questa parte sarebbe carino metterla in una funzione a se
	printf("\n\nI start sending to the server the file that are needed\n");
	DWORD read;
	int tot;
	int lunghezza = recInt(server);
	while (lunghezza != -10){
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		wstring wpath((wchar_t*)recNbytes(server, lunghezza, recvbuf));

		HANDLE file;
		
		for (std::list<Oggetto*>::iterator it = newconfig.begin(); it != newconfig.end(); ++it){
			Oggetto IT = *it;
			if (!IT.GetPath().compare(wpath)){
				file = IT.GetHandle();
				break;
			}
			
		}
		tot = 0;
		while (1){
			ReadFile(file, recvbuf, recvbuflen, &read, NULL);
			if (read != recvbuflen){
				sendNbytes(server, recvbuf, read);
				tot += read;
				break;
			}
			tot += read;
			sendNbytes(server, recvbuf, read);
		}
		lunghezza = recInt(server);
		SetFilePointer(file, 0, 0, 0);
	}
	sendInt(server, -10);
	if (recInt(server) == -10){
		printf("Sync terminated, no more file needed\n");
	}
	else{
		printf("ERROR during sync termination\n");
	}

	//pulizia
	
	PulisciLista(newconfig);
	PulisciLista(lastconfig);
	PulisciLista(missingfiles);
}

void Restore(SOCKET server, wstring wpath,string hash){

	printf("\nStarting the restore\n");
	sendInt(server, 50);
	
	sendInt(server, hash.size() + 1);
	sendNbytes(server, (char *)hash.c_str(), hash.size() + 1);
	sendInt(server, (wpath.size() + 1)*sizeof(wchar_t));
	sendNbytes(server, (char*)wpath.c_str(), (wpath.size() + 1)*sizeof(wchar_t));


	wchar_t* temp2= (wchar_t*)malloc((MAX_PATH)*sizeof(wchar_t));
	memcpy(temp2, wpath.c_str(), (wpath.size() + 1)*sizeof(wchar_t));
	PathRemoveFileSpec(temp2);
	SHCreateDirectoryExW(NULL, temp2, NULL);
	::free(temp2);

	std::wstring temp(wpath);
	temp.append(L".tempandhidden");
	HANDLE handle = CreateFileW(temp.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (handle == INVALID_HANDLE_VALUE){
		throw "the file in the restore has not been created";
	}
	if (recInt(server) == 999){
		printf("\n\n\ninvalid file name\n\n\n");
		throw "RESTORE invalid file name";
	}
	int size = recInt(server);
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int tot = 0;
	DWORD read;
	while (tot < (int)size){
		if (tot + recvbuflen < size){
			(char*)recNbytes(server, recvbuflen, recvbuf);
			WriteFile(handle, recvbuf, recvbuflen, &read, NULL);
		}
		else{
			(char*)recNbytes(server, size - tot, recvbuf);
			WriteFile(handle, recvbuf, size - tot, &read, NULL);
			tot += size - tot;
			break;
		}
		tot += DEFAULT_BUFLEN;
	}

	if (DeleteFileW(wpath.c_str()) == 0 && GetLastError()!=ERROR_FILE_NOT_FOUND){
		printf("deletetion error");
		CloseHandle(handle);
		DeleteFileW(temp.c_str());
		throw "RESTORE: impossibile eliminare il vecchio file";
	}

	CloseHandle(handle);
	MoveFileW(temp.c_str(), wpath.c_str());
	SetFileAttributesW(wpath.c_str(),FILE_ATTRIBUTE_NORMAL);
	

	if (recInt(server) == -50){
		sendInt(server, -50);
		if (recInt(server) == -50){
			printf("restore successfully completed\n");
		}
		else{
			printf("error detected in the restore\n");
			throw("error detected in the restore");
		}
	}
	else{
		printf("error detected in the restore\n");
		throw("error detected in the restore");
	}
	
	return;
}

list<Oggetto*> GetLastConfig(SOCKET server){
	printf("I send the #20 requested last config\n");
	sendInt(server, 20);
	list<Oggetto*>last;
	printf("I get the number of files in the last config\n");
	int n = recInt(server);

	char* Hash = recvFile(server);
	wchar_t* PathNameLast = (wchar_t*)recvFile(server);
	istringstream fhash(Hash);
	wistringstream fpath(PathNameLast);



	int t = 0;
	while (t<n){
		wchar_t buf1[DEFAULT_BUFLEN];
		char buf4[DEFAULT_BUFLEN];
		fpath.getline(buf1, DEFAULT_BUFLEN);
		fhash.getline(buf4, DEFAULT_BUFLEN);
		last.push_front(new Oggetto(buf1, L"", L"", buf4, 0, INVALID_HANDLE_VALUE));
		
		t++;
	}

	::free(Hash);
	::free(PathNameLast);

	if (recInt(server)!=-20){
		printf("\nAn error occurred while receiving last config");
	}
	return last;
}

void SerializeList(SOCKET server,list<Oggetto*> daser){
	//serialize list of files
	stringstream c;
	wstringstream b;

	for (std::list<Oggetto*>::iterator it = daser.begin(); it != daser.end(); ++it){

		Oggetto IT = *it;

		c.write(IT.GetHash().c_str(), IT.GetHash().size());
		c.write("\n", 1);

		b.write(IT.GetPath().c_str(), IT.GetPath().size());
		b.write(L"\n", 1);
		b.write(IT.GetName().c_str(), IT.GetName().size());
		b.write(L"\n", 1);
		b.write(IT.GetLastModified().c_str(), IT.GetLastModified().size());
		b.write(L"\n", 1);
	}
	printf("I send the number of obj in the list\n");
	sendInt(server, daser.size());

	printf("I send the size of first file and the file\n");
	sendInt(server, c.str().size());
	invFile(server, (char*)c.str().c_str(), c.str().size());

	printf("I send the size of second fileand the file\n");
	sendInt(server, b.str().size()*sizeof(wchar_t));
	invFile(server, (char*)b.str().c_str(), b.str().size()*sizeof(wchar_t));


	for (std::list<Oggetto*>::iterator it = daser.begin(); it != daser.end(); ++it){
		Oggetto IT = *it;
		DWORD  size = IT.GetSize();
		invFile(server, (char*)&size, sizeof(DWORD));
	}
	return;
}

list<Oggetto*> FilesDaMandare(list<Oggetto*>newconfig, list<Oggetto*> lastconfig){
	
	list<Oggetto*> da_mandare;
	
	for (std::list<Oggetto*>::iterator it = newconfig.begin(); it != newconfig.end(); ++it){
		Oggetto IT = *it;
		bool is_on_the_server = false;
		for (std::list<Oggetto*>::iterator it2 = lastconfig.begin(); it2 != lastconfig.end(); ++it2){
			Oggetto IT2 = *it2;
			if (!IT.GetPath().compare(IT2.GetPath()) && !IT.GetHash().compare(IT2.GetHash())){
				is_on_the_server = true;
			}
		}
		if (is_on_the_server == false){
			da_mandare.push_front(new Oggetto(IT.GetPath(), IT.GetName(), IT.GetLastModified(),IT.GetHash(), IT.GetSize(), INVALID_HANDLE_VALUE));
		}
	}
	return da_mandare;
}

int Register(SOCKET server){
	sendInt(server, 30);


	int c;
	DWORD read;
	char nome[500];
	char pass[500];

	ReadFile(hpipe, nome, 4, &read, NULL);
	c = *((int*)nome);
	ReadFile(hpipe, nome, 1, &read, NULL);
	ReadFile(hpipe, nome, c, &read, NULL);
	nome[read] = '\0';

	sendInt(server, strlen(nome) + 1);
	sendNbytes(server, nome, strlen(nome) + 1);

	ReadFile(hpipe, pass, 4, &read, NULL);
	c = *((int*)pass);
	ReadFile(hpipe, pass, 1, &read, NULL);
	ReadFile(hpipe, pass, c, &read, NULL);
	pass[read] = '\0';
	sendInt(server, strlen(pass) + 1);
	sendNbytes(server, pass, strlen(pass) + 1);

	return recInt(server);
}

int Login(SOCKET server){
	sendInt(server, 40);
	
	int c;
	DWORD read;
	char nome[500];
	char pass[500];

	ReadFile(hpipe, nome, 4, &read, NULL);
	c = *((int*)nome);
	ReadFile(hpipe, nome, 1, &read, NULL);
	ReadFile(hpipe, nome, c, &read, NULL);
	nome[read] = '\0';
	sendInt(server, strlen(nome)+1);
	sendNbytes(server, nome, strlen(nome)+1);
	
	ReadFile(hpipe, pass, 4, &read, NULL);
	c = *((int*)pass);
	ReadFile(hpipe, pass, 1, &read, NULL);
	ReadFile(hpipe, pass, c, &read, NULL);
	pass[read] = '\0';
	sendInt(server, strlen(pass)+1);
	sendNbytes(server, pass, strlen(pass)+1);
	return recInt(server);
}

void PulisciLista(std::list < Oggetto *>& a){
	Oggetto* p2;
	while (!a.empty()){
		p2 = a.front();
		if (p2->GetHandle() != INVALID_HANDLE_VALUE){
			if (!CloseHandle(p2->GetHandle())){
				std::cout << "HO CHIUSO UNA HANDLE MALE" << endl;
				std::cout << GetLastError() << endl;
			}
		}

		a.pop_front();
		delete p2;
		p2 = NULL;
	}
};
