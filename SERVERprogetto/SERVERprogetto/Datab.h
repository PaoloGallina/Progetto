#include "stdafx.h"
#include "sqlite3.h"
#include <string>
#include <list>
#include "Oggetto.h"

sqlite3* CreateDatabase();
void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash);
void InsertVER(sqlite3*db, std::wstring wpath, std::string hash,int ver);
void ReadFILES(sqlite3*db);
void ReadVERSIONE(sqlite3*db,int versione);
int esempio(sqlite3 *db);
void eliminaFILE(sqlite3* db, std::wstring wpath, std::string hash);
std::list<Oggetto*>  ListObjINeed(sqlite3 *,std::list<Oggetto*>);
void nuovaVersione(sqlite3* db, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere);
int GetUltimaVersione(sqlite3*db);