
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "sqlite3.h" 
#include <windows.h>
#include <string>
#include "Oggetto.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	//useless, right now only the creation of the table "use" it
	return 0;
}

sqlite3 * CreateDatabase(){
	sqlite3 *db;
	int  rc;
	char *sql;
	char *zErrMsg = 0;

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
	sql = "CREATE TABLE FILES( PATH BLOB NOT NULL, HASH TEXT NOT NULL,DATI  BLOB NOT NULL, PRIMARY KEY (PATH, HASH));";

	/* CREATING FILES TABLE */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE VERSIONS( PATH BLOB NOT NULL, VER INT NOT NULL,HASH  TEXT NOT NULL, PRIMARY KEY (PATH, VER));";

	/* CREATING VERSION TABLE */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	return db;
};

void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash){

	int  rc;
	std::string sql = "INSERT INTO FILES (PATH,HASH,DATI) VALUES (?1, ?2, ?3);";
	
	sqlite3_stmt* stm = NULL;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

	//IL FILE NON SI POTRA' PRENDERE IN QUESTO MODO!!
	std::ostringstream file_stringa_stream;
	std::ifstream ifs(wpath, std::ios::binary);
	file_stringa_stream << ifs.rdbuf();
	std::string file_string = file_stringa_stream.str();
	ifs.close();

	rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
	rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
	rc = sqlite3_bind_blob(stm, 3, file_string.c_str(), file_string.size(), SQLITE_STATIC);
	
	rc = sqlite3_step(stm);

	if (rc != SQLITE_DONE){
		fprintf(stderr, "SQL error: %d... 19? maybe you insered twice same entry!\n",rc);
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}
	
	sqlite3_finalize(stm);
};

void InsertVER(sqlite3*db, std::wstring wpath, std::string hash,int ver){
	//Just a stub
	int  rc;
	std::string sql = "INSERT INTO VERSIONS (PATH,VER,HASH) VALUES (?1, ?2, ?3);";

	sqlite3_stmt* stm = NULL;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

	rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
	rc = sqlite3_bind_int(stm, 2, ver);
	rc = sqlite3_bind_text(stm, 3, hash.c_str(), hash.size(), SQLITE_STATIC);
	

	rc = sqlite3_step(stm);

	if (rc != SQLITE_DONE){
		fprintf(stderr, "nuova versione non inserita!\n", rc);
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_finalize(stm);

	return;
};

void ReadFILES(sqlite3*db){
	int rc;

	/* Create SQL statement */
	std::string sql = "SELECT * from FILES";

	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_step(stm);

	std::wstring path;
	std::string hash;
	std::string file;
	std::wcout << L"FILE PRESENTI NEL DB \n----------------------------\n";
	while (rc == 100){
		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));
		hash = std::string((char*)sqlite3_column_text(stm, 1));
		file = std::string((char*)sqlite3_column_blob(stm, 2), sqlite3_column_bytes(stm, 2));
		std::wcout << path.c_str() ;
		std::wcout << "\n----------------------------"<<endl;
		rc = sqlite3_step(stm);
	}

	
	std::ofstream ofs(path, std::ios::binary);
	ofs << file;
	std::ofstream ifs(L"C:\\Users\\Paolo\\Desktop\\PROVA2\\VOGLIO RIVOLARE.wmv", std::ios::binary);
	ifs << file;

	sqlite3_finalize(stm);///POI PROVA A SPOSTARLO
	ofs.close();
	return;
}

list < Oggetto *> ListObjINeed(sqlite3 *db, list < Oggetto *> files, int versione){
	//this function takes as input the list of the files in the filesystem and 
	//has the porpose to give back the list of the missing (or modified files);
	int rc;
	list < Oggetto *> OggettiCheVorrei;
	

	/* Create SQL statement */
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_int(stm, 1, versione);
	rc = sqlite3_step(stm);

	std::wstring path;
	std::string hash;
	int ver;
	wcout << L"ULTIMA CONFIGURAZIONE PRESENTE\n--------------------" << endl;
	while (rc == 100){
		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));		
		hash = std::string((char*)sqlite3_column_text(stm, 2));
		ver  = sqlite3_column_int(stm, 1);
		//NON E' VENUTO NON AGGIUNTI QUELLI NUOVI E MESSO L?UNICO UGUALE!!! CAZZO!
			//per ogni file nel db dell'ultima versione controllo se è nella lista e se è stato modificato l'hash
			Oggetto* p2;
			int i = files.size();
			for (int t = 0; t < i; t++){
				while (!files.empty()){
					p2 = files.front();
					if (path.compare(p2->GetPath())){
						if (hash.compare(p2->GetHash())){
							files.pop_front();   //se l'hash è uguale non mi interessano
						}
						else{
							OggettiCheVorrei.push_front(new Oggetto(p2));
							files.pop_front();					/// se l'hash è diverso la metto nella lista di quelli che mi servono!
						}

					}
				}
			}
		
		wcout << path.c_str();
		wcout << L"VERSIONE         "+ver << endl;

		rc = sqlite3_step(stm);
	}
	//quelli che non ho trovato saranno sicuramente nuovi e quindi mi interessano!
	Oggetto* p2;
	while (!files.empty()){
		p2 = files.front();
		OggettiCheVorrei.push_front(new Oggetto(p2));
		files.pop_front();
	}


	rc = sqlite3_finalize(stm);	
	return OggettiCheVorrei;
}

int esempio()
{
	sqlite3 *db;

	/* Open database */
	db = CreateDatabase();

	InsertFILE(db,L"C:\\Users\\Paolo\\Desktop\\PROVA2\\VOGLIO RIVOLARE.wmv", "hash_esempio");
	
	ReadFILES(db);
	 //// MEMORY LEAKKKKS
	 //// PATH STRAMBI SONO O NON SONO UN PROBLEMA????

	sqlite3_close(db);

	system("pause");
	return 0;
}