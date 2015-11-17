#include "stdafx.h"
#include "sqlite3.h"
#include <string>
#include <list>
#include "Oggetto.h"

sqlite3* CreateDatabase(std::string nome);

void InsertFILE(sqlite3*db,SOCKET client, std::wstring wpath, std::string hash,int versione);
void InsertVER(sqlite3*db, std::wstring wpath, std::string hash, std::string last, int ver);
int GetUltimaVersione(sqlite3*db);
std::list<Oggetto*> AllFiles(sqlite3*db);
void ReadFILES(sqlite3*db);
void ReadVERSIONE(sqlite3*db,int versione);
void eliminaFILE(sqlite3* db, std::wstring wpath, std::string hash);
void nuovaVersione(sqlite3* db,SOCKET client, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere);
int file_cancellati(sqlite3* db, int);
void PulisciDB(sqlite3* db);
std::list<Oggetto*> LastVersion(sqlite3*db);
std::list<Oggetto*> Version(sqlite3*db, int ver);
void SendVersions(SOCKET client, std::string nome);