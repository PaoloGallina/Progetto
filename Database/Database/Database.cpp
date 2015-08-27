// Database.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "sqlite3.h" 
#include <windows.h>
#include <string>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("--------------------------\n");
	return 0;
}


void CreateDatabase(){
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open("test2.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE FILES( PATH TEXT NOT NULL, HASH TEXT NOT NULL," \
	      "DATI  BLOB NOT NULL, VERSIONE INT, ORALASTMOD    REAL, PRIMARY KEY (PATH, HASH));";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
};

void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash){

	char *zErrMsg = 0;
	int  rc;
	std::string sql="";
	/* Create SQL statement */
	std::string path(wpath.begin(),wpath.end());
	
	sql=sql.append("INSERT INTO FILES (PATH,HASH,DATI,VERSIONE,ORALASTMOD) VALUES ('")+ path.data() + "', '" + hash + "', 'DATA', 5, NULL );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}

};

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	
	CreateDatabase();
	/* Open database */
	rc = sqlite3_open("test2.db", &db);
	
	InsertFILE(db, L"fuck", "the system");
	/* Create SQL statement */
	sql = "SELECT * from FILES";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);

	system("pause");
	return 0;
}