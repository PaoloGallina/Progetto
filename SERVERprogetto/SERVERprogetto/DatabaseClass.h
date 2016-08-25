#pragma once


#include "stdafx.h"
#include "sqlite3.h"
#include <string>
#include <list>
#include "Sock.h"
#include "Oggetto.h"

class DatabaseClass
{

public:
	void nuovaVersione(SOCKET client, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere, char* passw);
	int GetUltimaVersione();
	int file_cancellati(int);
	std::list<Oggetto*> AllFiles();
	std::list<Oggetto*> LastVersion();
	std::list<Oggetto*> Version( int ver);
	void SendVersions(SOCKET client,char* passw);
	void Restore(SOCKET client, char* passw);
	void Register( std::string pass);
	std::string RetrivePass();

	DatabaseClass(std::string nome);
	~DatabaseClass();

private:

	void InsertFILE(SOCKET client, std::wstring wpath, std::string hash, int versione, DWORD size, std::wstring last, char* passw);
	void InsertVER(std::wstring wpath, std::string hash, std::string last, std::wstring lastfile, int ver);
	void eliminaFILE(std::wstring wpath, int ver);
	void PulisciDB();

	sqlite3* db;
	std::string nome;
};

