
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


	std::ostringstream sstringa;

	std::ifstream ifs("C:\\Users\\Paolo\\Desktop\\PROVA2\\VIDEO.wmv", std::ios::binary);
	sstringa << ifs.rdbuf();
	ifs.close();

	std::string sql = "INSERT INTO FILES (PATH,HASH,DATI,VERSIONE,ORALASTMOD) VALUES ('Primo', 'ssssss ', ?3 , 15, NULL );";
	
	
	sqlite3_stmt* stm = NULL;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

	std::string A = sstringa.str();
	rc=sqlite3_bind_blob(stm,3, A.c_str(), A.size(),SQLITE_STATIC);
	


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
	std::string codes;
	 while(rc == 100){
		 
		 //std::cout << (char*)sqlite3_column_blob(stm, 0)<<std::endl;
		 //std::cout << (char*)sqlite3_column_blob(stm, 1) << std::endl;
		  codes= std::string((char*)sqlite3_column_blob(stm, 2), sqlite3_column_bytes(stm, 2));
	
		 
		 rc = sqlite3_step(stm);
	}
	
	 std::ofstream ofs("C:\\Users\\Paolo\\Desktop\\PROVA2\\VOGLIO VOLARE.wmv", std::ios::binary);

	 //// MEMORY LEAKKKKS
	//// PATH STRAMBI SONO O NON SONO UN PROBLEMA????
	 /// NO SERIALIZE!!! SOCKEtS?
	 ofs << codes;
	 ofs.close();

	sqlite3_close(db);


	system("pause");
	return 0;
}