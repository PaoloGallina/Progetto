
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
	sql = "CREATE TABLE FILES( PATH BLOB NOT NULL, HASH TEXT NOT NULL,DATI  BLOB NOT NULL, VER INT NOT NULL, PRIMARY KEY (PATH, HASH));";

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

int GetUltimaVersione(sqlite3*db){
	int rc;
	/* Create SQL statement */
	std::string sql = "SELECT max(VER) FROM VERSIONS";
	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_step(stm);
	
	int temp=0;
	if (rc == 100){
		temp = sqlite3_column_int(stm, 0);
	}
	else{
		temp = 0;
	}

	rc = sqlite3_finalize(stm);
	return temp;
}

list < Oggetto *> CheFILEvoglio(sqlite3 *db, list < Oggetto *> files){
	//this function takes as input the list of the files in the filesystem and 
	//has the porpose to give back the list of the missing (or modified files);
	int rc;
	int versione = GetUltimaVersione(db);
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
			if (!wcscmp(path.c_str(), p2->GetPath().c_str())){
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

void InsertFILE(sqlite3*db, std::wstring wpath, std::string hash,int versione){

	int  rc=0;
	std::string sql = "INSERT INTO FILES (PATH , HASH , DATI , VER) VALUES (?1, ?2, ?3, ?4);";
	
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
	rc = sqlite3_bind_int(stm, 4, versione);

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
	int ver;
	std::wcout << L"\n\n\nFILE PRESENTI NEL DB \n----------------------------\n";
	if (rc != 100){
		std::wcout << L"   NESSUN FILE PRESENTE" << endl;
	}
	while (rc == 100){

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));
		hash = std::string((char*)sqlite3_column_text(stm, 1));
		file = std::string((char*)sqlite3_column_blob(stm, 2), sqlite3_column_bytes(stm, 2));
		ver = (sqlite3_column_int(stm, 3));

		std::wcout << path.c_str()<<"       COMPARE IN ="<<ver<< endl ;
		
		//wcout << L"    HASH    " << hash.c_str() << endl;
		
	//	std::ofstream ofs(path, std::ios::binary);
	//	ofs << file;
	//	ofs.close();
		rc = sqlite3_step(stm);
	}

	
	

	sqlite3_finalize(stm);///POI PROVA A SPOSTARLO
	
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
	wcout << L"\n\n\nFILES NELLA CONFIGURAZIONE  --->  "<<versione<<"\n--------------------" << endl;
	if (rc != 100){
		wcout << L"NESSUN FILE PRESENTE NELLA CONFIGURAZIONE \n" << endl;
	}
	while (rc == 100){

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0));
		ver = sqlite3_column_int(stm, 1);
		hash = std::string((char*)sqlite3_column_text(stm, 2));

		wcout << path.c_str()  << endl;
	//	wcout << L"    HASH    " << hash.c_str() << endl;

		rc = sqlite3_step(stm);

	}

	rc = sqlite3_finalize(stm);
	return;
}

void eliminaFILE(sqlite3* db,wstring wpath,string hash){
	int rc;

	/* Create SQL statement */
	std::string sql = "DELETE from FILES where PATH=?1 and HASH=?2";

	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
	rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
	rc = sqlite3_step(stm);

	sqlite3_finalize(stm);

};

void pulisciDATAB(sqlite3* db){
	//questa funzione avrà il compito di pulire il database da tutte le versioni troppo vecchie
	//come faccio a capire se i file sono vecchi? Ho bisogno di un timetag per salvare quelli più recenti!
	return;
}

int file_cancellati(sqlite3* db, int val){
	//se i file nel filesystem sono lo stesso numero (VAL è appunto il numero di file) 
	//o uno è stato aggiunto e uno rimosso (e quindi la lista da_chiedere non è vuota)
	//oppure non c'è stata alcuna modifica        
	int versione = GetUltimaVersione(db);
	int rc;
	int counter=0;
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_int(stm, 1, versione);
	rc = sqlite3_step(stm);
	while (rc == 100){

		counter++;
		rc = sqlite3_step(stm);

	}


	rc = sqlite3_finalize(stm);
	return val - counter;
};

void nuovaVersione(sqlite3* db, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere){
	//Devo prendere i File dalla "da_chiedere" e creare l'entry corrispondente  (in FILES)
	//Per tutti i File in listaobj e mette l'entry in VERSIONS e aggiornare l'ultima versione in cui sono stati utilizzati
	//Questo deve essere fatto fra un begin e un commit!
	
	int Versione = GetUltimaVersione(db);
	Versione++;
	for (std::list < Oggetto *>::const_iterator ci2 = da_chiedere.begin(); ci2 != da_chiedere.end(); ++ci2){
		InsertFILE(db, (*ci2)->GetPath(), (*ci2)->GetHash(), Versione);
	}

	for (std::list < Oggetto *>::const_iterator ci = listaobj.begin(); ci != listaobj.end(); ++ci){
		InsertVER(db, (*ci)->GetPath(), (*ci)->GetHash(), Versione);
		
		std::string sql = "UPDATE FILES SET VER = ?3 where hash=?2 and path=?1";
		int rc;
		sqlite3_stmt* stm;
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		wstring wpath = ((*ci)->GetPath());
		string hash = ((*ci)->GetHash());

		rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
		rc = sqlite3_bind_int(stm, 3, Versione);

		rc = sqlite3_step(stm);
		sqlite3_finalize(stm);
	}
}

int esempio(sqlite3 *db)
{
	
	int UltimaVersione = GetUltimaVersione(db);

	InsertFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	InsertFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	eliminaFILE(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076");


	//InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a2.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	//InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\a.txt", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	//InsertVER(db, L"C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", "94d148fc7cf925f1d1ad97873930079da668e14033cda3545b891bac192ee076", UltimaVersione);
	ReadFILES(db);
	ReadVERSIONE(db,324);
	return 0;
}