#include "stdafx.h"
#include "sqlite3.h"
#include <string>
#include <list>
#include "Oggetto.h"

sqlite3* CreateDatabase();
void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash);
void InsertVER(sqlite3*db, std::wstring wpath, std::string hash, int ver);
void ReadFILES(sqlite3*db);
void ReadVERSIONE(sqlite3*db,int);
int esempio(sqlite3 *db,int);
void eliminaFILE(sqlite3* db, std::wstring wpath, std::string hash);
std::list<Oggetto*>  ListObjINeed(sqlite3 *,std::list<Oggetto*> , int );
