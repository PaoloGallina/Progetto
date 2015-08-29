#include "stdafx.h"
#include "sqlite3.h"
#include <string>

sqlite3 * CreateDatabase();
void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash);
void ReadFILES(sqlite3*db);
int esempio();