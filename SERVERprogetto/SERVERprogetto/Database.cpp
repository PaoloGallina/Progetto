
#include "stdafx.h"
#include "Sock.h"
#include <stdio.h>
#include <iostream>
#include "sqlite3.h" 
#include <string>
#include "Oggetto.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <exception>

using namespace std;

//tutto questo può diventare una classe singleton con nome datab, versione ecc salvati in un file

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	//useless, right now only the creation of the table "use" it
	return 0;
}

sqlite3 * CreateDatabase(std::string nome){
	sqlite3 *db;
	int  rc;
	char *sql;
	char *zErrMsg = 0;

	/* Open database */
	rc = sqlite3_open(nome.c_str(), &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE FILES( PATH BLOB NOT NULL, HASH TEXT NOT NULL,DATI  BLOB, VER INT NOT NULL, PRIMARY KEY (PATH, HASH));";

	/* CREATING FILES TABLE */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
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
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}
	
	/* Create SQL statement */
	sql = "CREATE TABLE CREDENTIAL( USER TEXT NOT NULL, PASS TEXT NOT NULL, PRIMARY KEY (USER));";

	/* CREATING VERSION TABLE */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
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
	int temp = 0;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_step(stm);

		
		if (rc == 100){
			temp = sqlite3_column_int(stm, 0);
		}
		else{
			temp = 0;
		}
	}
	catch (...){

		rc = sqlite3_finalize(stm);
		throw "error during getUltimaVersione";
	}
	rc = sqlite3_finalize(stm);
	return temp;
}

list<Oggetto*> LastVersion(sqlite3*db){
	int rc;
	int versione = GetUltimaVersione(db);
	list < Oggetto *> last;


	/* Create SQL statement */
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_int(stm, 1, versione);
		rc = sqlite3_step(stm);

		std::wstring path;
		std::string hash;
		int ver;
		while (rc == 100){

			path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0) / sizeof(TCHAR));
			ver = sqlite3_column_int(stm, 1);
			hash = std::string((char*)sqlite3_column_text(stm, 2));

			Oggetto* p2 = new Oggetto(path, L"", L"", hash, 0, INVALID_HANDLE_VALUE);
			last.push_front(p2);
			rc = sqlite3_step(stm);
		}
	}
	catch (...){
		sqlite3_finalize(stm);
		throw"error during Last version";
	}
	sqlite3_finalize(stm);
	return last;
}

void InsertFILE(sqlite3*db,SOCKET client, std::wstring wpath, std::string hash,int versione,DWORD size){

	int  rc=0;
	std::string sql = "INSERT INTO FILES (PATH , HASH , DATI , VER) VALUES (?1, ?2, ?3, ?4);";
	
	sqlite3_stmt* stm = NULL;
	sqlite3_stmt* stm2 = NULL;
	sqlite3_blob *BLOB=NULL;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

		rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(wchar_t), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
		rc = sqlite3_bind_zeroblob(stm, 3, size);
		rc = sqlite3_bind_int(stm, 4, versione);

		rc = sqlite3_step(stm);

		if (rc != SQLITE_DONE){
			fprintf(stderr, " This file was already present in the database!\n", rc);
		}
		else{
			fprintf(stdout, "Record FILE created successfully\n");
		}




		std::string sql2 = "SELECT ROWID FROM FILES WHERE HASH=?1 and PATH=?2;";

		rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stm2, NULL);
		rc = sqlite3_bind_text(stm2, 1, hash.c_str(), hash.size(), SQLITE_STATIC);
		rc = sqlite3_bind_blob(stm2, 2, wpath.c_str(), wpath.size()*sizeof(wchar_t), SQLITE_STATIC);
		rc = sqlite3_step(stm2);
		sqlite_int64 rowid = sqlite3_column_int64(stm2, 0);


		rc = sqlite3_blob_open(db, "main", "FILES", "DATI", rowid, 1, &BLOB);
		if (rc == 1){
			cout << sqlite3_errmsg(db) << endl;
		}

		sendInt(client, (wpath.size() + 1)*sizeof(wchar_t));
		sendNbytes(client, (char*)wpath.c_str(), (wpath.size() + 1)*sizeof(wchar_t));

		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int tot = 0;

		while (tot < (int)size){
			if (tot + recvbuflen < size){
				rc = sqlite3_blob_write(BLOB, (char*)recNbytes(client, recvbuflen, recvbuf), recvbuflen, tot);
			}
			else{
				rc = sqlite3_blob_write(BLOB, (char*)recNbytes(client, size - tot, recvbuf), size - tot, tot);
				tot += size - tot;
				break;
			}
			tot += DEFAULT_BUFLEN;
		}
	}
	catch (...){
		sqlite3_finalize(stm);
		sqlite3_finalize(stm2);
		sqlite3_blob_close(BLOB);
		throw "Error during insert file";
	}
	sqlite3_finalize(stm);
	sqlite3_finalize(stm2);
	sqlite3_blob_close(BLOB);
	
};

void InsertVER(sqlite3*db, std::wstring wpath, std::string hash,int ver){
	//Just a stub
	int  rc;
	std::string sql = "INSERT INTO VERSIONS (PATH,VER,HASH) VALUES (?1, ?2, ?3);";

	sqlite3_stmt* stm = NULL;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

		rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
		rc = sqlite3_bind_int(stm, 2, ver);
		rc = sqlite3_bind_text(stm, 3, hash.c_str(), hash.size(), SQLITE_STATIC);


		rc = sqlite3_step(stm);

		if (rc != SQLITE_DONE){
			printf("nuova versione non inserita!\n");
		}
		else{
			printf("Record VERSION created successfully\n");
		}
	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during insert VER";
	}
	sqlite3_finalize(stm);

	return;
}

void ReadFILES(sqlite3*db){
	int rc;

	/* Create SQL statement */
	std::string sql = "SELECT PATH,HASH,VER from FILES";
	//std::string sql = "SELECT PATH,HASH,VER,DATI from FILES";

	sqlite3_stmt* stm;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_step(stm);

		std::wstring path;
		std::string hash;
		//std::string file;
		int ver;
		std::wcout << L"\n\n\nFILE PRESENTI NEL DB \n----------------------------\n";
		if (rc != 100){
			std::wcout << L"   NESSUN FILE PRESENTE" << endl;
		}
		while (rc == 100){

			path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0) / sizeof(TCHAR));
			hash = std::string((char*)sqlite3_column_text(stm, 1));
			ver = (sqlite3_column_int(stm, 2));

			//file = std::string((char*)sqlite3_column_blob(stm, 3), sqlite3_column_bytes(stm, 3));
			//std::ofstream ofs(path, std::ios::binary);
			//ofs << file;
			//std::ofstream ifs(path + L".wmv", std::ios::binary);
			//ifs << file;

			std::wcout << ver << " : " << hash.c_str() << endl;
			rc = sqlite3_step(stm);
		}




	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during read file";
	}
	sqlite3_finalize(stm);
	
	return;
}

void ReadVERSIONE(sqlite3 *db, int versione){
	int rc;
	/* Create SQL statement */
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	try{
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

		path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0)/sizeof(TCHAR));
		ver = sqlite3_column_int(stm, 1);
		hash = std::string((char*)sqlite3_column_text(stm, 2));

	//	wcout << path  << endl;
		wcout << L"    HASH    " << hash.c_str() << endl;

		rc = sqlite3_step(stm);

	}
	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during read file";
	}
	rc = sqlite3_finalize(stm);
	return;
}

void eliminaFILE(sqlite3* db,wstring wpath,int ver){
	int rc;

	/* Create SQL statement */
	std::string sql = "DELETE from FILES where PATH=?1 and VER=?2";

	sqlite3_stmt* stm;
	try{
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
	rc = sqlite3_bind_int(stm, 2, ver);
	rc = sqlite3_step(stm);
	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during read Eliminafile";
	}
	sqlite3_finalize(stm);

};

void PulisciDB(sqlite3* db){

	int rc;
	int Versione = GetUltimaVersione(db);
	std::string sql = "DELETE FROM VERSIONS WHERE VER<?1";
	sqlite3_stmt* stm=NULL;
	sqlite3_stmt* stm2=NULL;
	try{
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_int(stm, 1, Versione - 2);
	rc = sqlite3_step(stm);
	

	std::string sql2 = "SELECT PATH,MIN(VER) FROM FILES GROUP BY PATH HAVING count(*)>3";
	
	rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stm2, NULL);
	rc = sqlite3_step(stm2);
	while (rc == 100){

		wstring wpath = std::wstring((TCHAR*)sqlite3_column_blob(stm2, 0), sqlite3_column_bytes(stm2, 0) / sizeof(TCHAR));
		int ver = sqlite3_column_int(stm2, 1);
		sqlite3_finalize(stm2);
		eliminaFILE(db, wpath, ver);

		rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stm2, NULL);
		rc = sqlite3_step(stm2);
	}
	}
	catch (...){
		sqlite3_finalize(stm);
		sqlite3_finalize(stm2);
		throw "error pulisci DB";
	}
	sqlite3_finalize(stm);
	sqlite3_finalize(stm2);

}

void nuovaVersione(sqlite3* db,SOCKET client, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere){
	char *zErrMsg = 0;
	try{
		sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
		int Versione = GetUltimaVersione(db);
		Versione++;
		for (std::list < Oggetto *>::const_iterator ci2 = da_chiedere.begin(); ci2 != da_chiedere.end(); ++ci2){
			InsertFILE(db,client, (*ci2)->GetPath(), (*ci2)->GetHash(), Versione, (*ci2)->GetSize());
		}

		for (std::list < Oggetto *>::const_iterator ci = listaobj.begin(); ci != listaobj.end(); ++ci){
			InsertVER(db, (*ci)->GetPath(), (*ci)->GetHash(), Versione);


			std::string sql = "UPDATE FILES SET VER = ?3 where hash=?2 and path=?1";
			int rc;
			sqlite3_stmt* stm;
			try{
			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
			wstring wpath = ((*ci)->GetPath());
			string hash = ((*ci)->GetHash());

			rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
			rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
			rc = sqlite3_bind_int(stm, 3, Versione);

			//qui il programma richiede di colpo tantissima memoria.. ma è colpa di sqlite3
			rc = sqlite3_step(stm);
			}
			catch (...){
				sqlite3_finalize(stm);
				throw "error during update VER";
			}
			sqlite3_finalize(stm);

		}

		PulisciDB(db);
		sqlite3_exec(db, "vacuum;", NULL, NULL, NULL);
		sqlite3_exec(db, "END TRANSACTION;", callback, NULL, &zErrMsg);
		if (zErrMsg != nullptr){
			throw "error during the commit-->ROLLBACK";
		}

		sendInt(client, -10);
		if (recInt(client) != -10){
			printf("sync problem, not terminated correctly");
			throw "sync problem, not terminated correctly";
		}
		else{
			sendInt(client, -10);
			printf("sync is terminated no more files are needed");
		}

		wcout << L"TRANSACTION COMMITTED" << endl;

	}
	catch (...){
		wcout << L"ROLLBACK" << endl;
		sqlite3_exec(db, "ROLLBACK;", callback, NULL, &zErrMsg);
		if (zErrMsg != nullptr){
			throw "error during the ROLLBACK";
		}
		throw "errore durante la creazione di una nuova versione";
	}
}

int file_cancellati(sqlite3* db, int val){
	
	int versione = GetUltimaVersione(db);
	int rc;
	int counter = 0;
	
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	try{
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
	rc = sqlite3_bind_int(stm, 1, versione);
	rc = sqlite3_step(stm);
	while (rc == 100){
		counter++;
		rc = sqlite3_step(stm);
	}

	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during file eliminati";
	}
	rc = sqlite3_finalize(stm);

	return val - counter;
};
