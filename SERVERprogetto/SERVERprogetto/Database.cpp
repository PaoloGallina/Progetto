
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

//tutto questo può diventare una classe singleton con nome datab, versione ecc salvati in un file

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
	std::wcout << L"\n\n\nFILE PRESENTI NEL DB \n----------------------------\n";
	if (rc != 100){
		std::wcout << L"   NESSUN FILE PRESENTE" << endl;
	}
	while (rc == 100){

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));
		hash = std::string((char*)sqlite3_column_text(stm, 1));
		file = std::string((char*)sqlite3_column_blob(stm, 2), sqlite3_column_bytes(stm, 2));
		std::wcout << path.c_str() << endl ;
		wcout << L"    HASH    " << hash.c_str() << endl;

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

void ReadVERSIONE(sqlite3 *db, int versione){
	int rc;

	/* Create SQL statement */
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_int(stm, 1, versione);
	rc = sqlite3_step(stm);

	std::wstring path;
	std::string hash;
	int ver;
	wcout << L"\n\n\nULTIMA CONFIGURAZIONE PRESENTE\n--------------------" << endl;
	if (rc != 100){
		wcout << L"\n\n\nNESSUN FILE PRESENTE NELLA CONFIGURAZIONE\n--------------------" << endl;
	}
	while (rc == 100){

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));
		ver = sqlite3_column_int(stm, 1);
		hash = std::string((char*)sqlite3_column_text(stm, 2));

		wcout << path.c_str() << L"    VERSIONE " << ver << endl;
		wcout << L"    HASH    " << hash.c_str() << endl;

		rc = sqlite3_step(stm);

	}

	rc = sqlite3_finalize(stm);
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
	while (rc == 100){

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));		
		ver = sqlite3_column_int(stm, 1);
		hash = std::string((char*)sqlite3_column_text(stm, 2));
		
			Oggetto* p2;
			int i = files.size();
			for (int t = 0; t < i; t++){
					p2 = files.front();
					if (!wcscmp(path.c_str(),p2->GetPath().c_str())){
						if (!strcmp(hash.c_str(), p2->GetHash().c_str())){
							files.pop_front();   //se l'hash è uguale non mi interessano
							//non faccio il delete, ci penserà chi ha creato la lista!
						}
						else{
							OggettiCheVorrei.push_front(new Oggetto(p2));
							files.pop_front();					/// se l'hash è diverso la metto nella lista di quelli che mi servono!
							//non faccio il delete, ci penserà chi ha creato la lista!
						}
					}
					else{
						files.pop_front();					
						files.push_back(p2);
					}

				
				// SE NON TROVA IL FILE NON SUCCEDE NULLA!! DEVO USARE UN ITERATORE	
			}
		rc = sqlite3_step(stm);
	}
	//quelli che non ho trovato saranno sicuramente nuovi e quindi mi interessano!
	Oggetto* p2;
	while (!files.empty()){
		p2 = files.front();
		OggettiCheVorrei.push_front(new Oggetto(p2));
		files.pop_front();
		//non faccio il delete, ci penserà chi ha creato la lista!
	}


	rc = sqlite3_finalize(stm);	
	return OggettiCheVorrei;
}

void eliminaFILE(sqlite3* db,wstring wpath,string hash){
	int rc;

	/* Create SQL statement */
	std::string sql = "DELETE from FILES where path=?1 and hash=?2";

	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
	rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
	rc = sqlite3_step(stm);

	sqlite3_finalize(stm);

};

int esempio(sqlite3 *db,int UltimaVersione)
{
	

	/* Open database */
	InsertFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a1.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076");
	InsertFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076");
	InsertFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076");


	InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a2.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	eliminaFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076");
	ReadFILES(db);
	ReadVERSIONE(db, UltimaVersione);
	return 0;
}