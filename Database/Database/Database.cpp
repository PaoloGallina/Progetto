// Database.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "sqlite3.h" 
#include <windows.h>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>

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
	rc = sqlite3_open("test.db", &db);
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

	int  rc;


	std::wostringstream sstringa;

	std::string path(wpath.begin(), wpath.end());
	std::ifstream ifs("C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", std::ios::binary);
	sstringa << ifs.rdbuf();
	ifs.close();

	std::string sql = "INSERT INTO FILES (PATH,HASH,DATI,VERSIONE,ORALASTMOD) VALUES ('a', 'ssssss ', ?3 , 15, NULL );";
	
	
	sqlite3_stmt* stm = NULL;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

//	sstringa.str(L"capricci");
	std::wstring A = sstringa.str();
	const wchar_t* AA = A.c_str();
	rc=sqlite3_bind_blob(stm,3, AA, sizeof(wchar_t)*(wcslen((AA))+1),SQLITE_STATIC);
	//rc = sqlite3_bind_blob(stm, 3, &(a), sizeof(wchar_t)*50, SQLITE_STATIC);
	


	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n");
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}
	rc = sqlite3_step(stm);


};

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	const char* data = "Callback function called";
	
	CreateDatabase();
	/* Open database */
	rc = sqlite3_open("test.db", &db);
	std::wstring a = L"checazzo2";
	InsertFILE(db, a, "the system");
	
	/* Create SQL statement */
	std::string sql = "SELECT * from FILES";
	
	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_step(stm);
	
	wchar_t *AA= L"";

	 while(rc == 100){
		 std::wstring prrr=std::wstring((wchar_t*)sqlite3_column_blob(stm, 2));
		 std::wcout << prrr << std::endl;
		 std::cout << (char*)sqlite3_column_blob(stm, 1)<<std::endl;
		 std::cout << (char*)sqlite3_column_blob(stm, 0) << std::endl;
		 std::cout << sqlite3_column_int(stm, 3) << std::endl;
		 std::cout << sqlite3_column_bytes(stm, 2) << std::endl;
		 rc = sqlite3_step(stm);
	}
	

	sqlite3_close(db);


	system("pause");
	return 0;
}