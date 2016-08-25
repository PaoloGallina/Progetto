#include "stdafx.h"
#include "DatabaseClass.h"
#include "Sock.h"
#include <stdio.h>
#include <iostream>
#include "sqlite3.h" 
#include <string>
#include "Oggetto.h"
#include <chrono>
#include <string.h>
#include <fstream>
#include <sstream>
#include <list>
#include <exception>
#include <iomanip>
#include <ctime>

using namespace std;


DatabaseClass::DatabaseClass(std::string nome)
{
	sqlite3 *db;
	int  rc;
	char *sql;
	char *zErrMsg = 0;
	string temp = nome;
	temp.append(".db");
	/* Open database */
	rc = sqlite3_open(temp.c_str(), &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE FILES( PATH BLOB NOT NULL, HASH TEXT NOT NULL,DATI  BLOB, VER INT NOT NULL,LAST BLOB, PRIMARY KEY (PATH, HASH));";

	/* CREATING FILES TABLE */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE VERSIONS( PATH BLOB NOT NULL, VER INT NOT NULL,HASH  TEXT NOT NULL,LAST BLOB NOT NULL,LASTFILE BLOB NOT NULL, PRIMARY KEY (PATH, VER));";

	/* CREATING VERSION TABLE */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE CREDENTIAL( USER TEXT NOT NULL, PASS TEXT NOT NULL, PRIMARY KEY (USER));";

	/* CREATING VERSION TABLE */
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	this->db = db;
	this->nome = nome;
}

list<Oggetto*> DatabaseClass::Version(int ver){
	int rc;
	int versione = ver;
	list < Oggetto *> last;


	/* Create SQL statement */
	std::string sql = "SELECT * FROM VERSIONS WHERE VER=?1";
	sqlite3_stmt* stm;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_int(stm, 1, versione);
		rc = sqlite3_step(stm);

		std::wstring path;
		std::wstring lastfile;
		std::string hash;
		int ver;
		while (rc == 100){

			path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0) / sizeof(TCHAR));
			ver = sqlite3_column_int(stm, 1);
			hash = std::string((char*)sqlite3_column_text(stm, 2));
			lastfile = std::wstring((TCHAR*)sqlite3_column_blob(stm, 4), sqlite3_column_bytes(stm, 4) / sizeof(TCHAR));

			Oggetto* p2 = new Oggetto(path, L"", lastfile, hash, 0, INVALID_HANDLE_VALUE);
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

list<Oggetto*> DatabaseClass::LastVersion(){
	int rc;
	int versione = this->GetUltimaVersione();
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
		std::wstring lastfile;
		int ver;
		while (rc == 100){

			path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0) / sizeof(TCHAR));
			ver = sqlite3_column_int(stm, 1);
			hash = std::string((char*)sqlite3_column_text(stm, 2));
			lastfile = std::wstring((TCHAR*)sqlite3_column_blob(stm, 4), sqlite3_column_bytes(stm, 4) / sizeof(TCHAR));

			Oggetto* p2 = new Oggetto(path, L"", lastfile, hash, 0, INVALID_HANDLE_VALUE);
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

list<Oggetto*> DatabaseClass::AllFiles(){
	int rc;
	list < Oggetto *> last;


	/* Create SQL statement */
	std::string sql = "SELECT PATH,HASH,LAST FROM FILES";
	sqlite3_stmt* stm;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_step(stm);

		std::wstring path;
		std::wstring lastm;
		std::string hash;
		while (rc == 100){

			path = std::wstring((TCHAR*)sqlite3_column_blob(stm, 0), sqlite3_column_bytes(stm, 0) / sizeof(TCHAR));
			hash = std::string((char*)sqlite3_column_text(stm, 1));
			lastm = std::wstring((TCHAR*)sqlite3_column_blob(stm, 2), sqlite3_column_bytes(stm, 2) / sizeof(TCHAR));

			Oggetto* p2 = new Oggetto(path, L"", lastm, hash, 0, INVALID_HANDLE_VALUE);
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

string DatabaseClass::RetrivePass(){
	sqlite3_stmt* stm=nullptr;
	std::string passD="";
	try{
		std::string sql = "SELECT PASS FROM CREDENTIAL WHERE USER=?1";
		int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
		rc = sqlite3_step(stm);
		passD = std::string((char*)sqlite3_column_text(stm, 0));
		
	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during login";
	}
	sqlite3_finalize(stm);
	return passD;
	
}

void DatabaseClass::Restore(SOCKET client, char* passw){

	char hash[DEFAULT_BUFLEN];
	wchar_t path[DEFAULT_BUFLEN];

	int Lhash = recInt(client, passw);
	recNbytes(client, Lhash, hash, passw);
	int Lpath = recInt(client, passw);
	recNbytes(client, Lpath, (char*)path, passw);

	int rc;
	sqlite3_blob *BLOB = nullptr;
	sqlite3_stmt* stm = nullptr;
	try{
		/* Create SQL statement */
		std::string sql = "SELECT rowid from FILES where PATH=?1 and HASH=?2";


		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_blob(stm, 1, path, wcslen(path)*sizeof(TCHAR), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, hash, strlen(hash), SQLITE_STATIC);
		rc = sqlite3_step(stm);
		sqlite_int64 rowid = sqlite3_column_int64(stm, 0);



		rc = sqlite3_blob_open(db, "main", "FILES", "DATI", rowid, 0, &BLOB);
		if (rc == 1){
			std::cout << sqlite3_errmsg(db) << std::endl;
			sendInt(client, 999, passw);
			throw "invalid rowid";
		}
		else{ sendInt(client, 0, passw); }

		int size = sqlite3_blob_bytes(BLOB);
		sendInt(client, size, passw);
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int tot = 0;
		while (tot < size){
			if (tot + recvbuflen < size){
				int read = sqlite3_blob_read(BLOB, recvbuf, recvbuflen, tot);
				sendNbytes(client, recvbuf, recvbuflen, passw);
			}
			else{
				int read = sqlite3_blob_read(BLOB, recvbuf, size - tot, tot);
				sendNbytes(client, recvbuf, size - tot, passw);
				tot += size - tot;
				break;
			}
			tot += DEFAULT_BUFLEN;
		}


	}
	catch (...){
		sqlite3_finalize(stm);
		sqlite3_blob_close(BLOB);
		throw "error during restore";
	}
	sqlite3_finalize(stm);
	sqlite3_blob_close(BLOB);

	sendInt(client, -50, passw);
	if (recInt(client, passw) != -50){
		printf("error detected in the restore\n");
		throw("error detected in the restore");
	}
	else{
		printf("restore successfully completed\n");
	}
	sendInt(client, -50, passw);
}

void DatabaseClass::InsertFILE(SOCKET client, std::wstring wpath, std::string hash, int versione, DWORD size, wstring last, char* passw){

	int  rc = 0;
	std::string sql = "INSERT INTO FILES (PATH , HASH , DATI , VER, LAST) VALUES (?1, ?2, ?3, ?4,?5);";

	sqlite3_stmt* stm = NULL;
	sqlite3_stmt* stm2 = NULL;
	sqlite3_blob *BLOB = NULL;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

		rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(wchar_t), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, hash.c_str(), hash.size(), SQLITE_STATIC);
		rc = sqlite3_bind_zeroblob(stm, 3, size);
		rc = sqlite3_bind_int(stm, 4, versione);
		rc = sqlite3_bind_blob(stm, 5, last.c_str(), last.size()*sizeof(wchar_t), SQLITE_STATIC);

		rc = sqlite3_step(stm);

		if (rc != SQLITE_DONE){
			sqlite3_finalize(stm);
			fprintf(stderr, " This file was already present in the database!\n", rc);
			return;
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
			throw "Errore nell'apertura del blob";
		}

		sendInt(client, (wpath.size() + 1)*sizeof(wchar_t), passw);
		sendNbytes(client, (char*)wpath.c_str(), (wpath.size() + 1)*sizeof(wchar_t), passw);

		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int tot = 0;

		while (tot < (int)size){
			if (tot + recvbuflen < size){
				rc = sqlite3_blob_write(BLOB, (char*)recNbytes(client, recvbuflen, recvbuf, passw), recvbuflen, tot);
			}
			else{
				rc = sqlite3_blob_write(BLOB, (char*)recNbytes(client, size - tot, recvbuf, passw), size - tot, tot);
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

void DatabaseClass::nuovaVersione(SOCKET client, std::list < Oggetto *> listaobj, std::list < Oggetto *> da_chiedere, char* passw){
	char *zErrMsg = 0;
	try{
		sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
		int Versione = GetUltimaVersione();
		Versione++;
		for (std::list < Oggetto *>::const_iterator ci2 = da_chiedere.begin(); ci2 != da_chiedere.end(); ++ci2){
			InsertFILE(client, (*ci2)->GetPath(), (*ci2)->GetHash(), Versione, (*ci2)->GetSize(), (*ci2)->GetLastModified(), passw);
		}

		sendInt(client, -10, passw);
		//message to keep alive the connection
		recInt(client, passw);
		printf("Now i update all version file");

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[180];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", timeinfo);
		std::string last(buffer);

		for (std::list < Oggetto *>::const_iterator ci = listaobj.begin(); ci != listaobj.end(); ++ci){
			InsertVER((*ci)->GetPath(), (*ci)->GetHash(), last, (*ci)->GetLastModified(), Versione);

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
		//message to keep alive the connection
		recInt(client, passw);
		printf("Pulizia DB\n");
		PulisciDB();

		//message to keep alive the connection

		recInt(client, passw);
		sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
		if (zErrMsg != nullptr){
			std::wcerr << std::endl << zErrMsg << std::endl;
			throw "error during the commit-->ROLLBACK";
		}

		wcout << L"\nTRANSACTION COMMITTED" << endl;

	}
	catch (...){
		wcout << L"ROLLBACK" << endl;
		sqlite3_exec(db, "ROLLBACK;", NULL, NULL, &zErrMsg);
		if (zErrMsg != nullptr){
			throw "error during the ROLLBACK";
		}
		throw "errore durante la creazione di una nuova versione";
	}


	//sqlite3_exec(db, "vacuum;", NULL, NULL, NULL);

}

void DatabaseClass::InsertVER(std::wstring wpath, std::string hash, std::string last, std::wstring lastfile, int ver){

	int  rc;
	std::string sql = "INSERT INTO VERSIONS (PATH,VER,HASH,LAST,LASTFILE) VALUES (?1, ?2, ?3,?4,?5);";

	sqlite3_stmt* stm = NULL;
	try{
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);

		rc = sqlite3_bind_blob(stm, 1, wpath.c_str(), wpath.size()*sizeof(TCHAR), SQLITE_STATIC);
		rc = sqlite3_bind_int(stm, 2, ver);
		rc = sqlite3_bind_text(stm, 3, hash.c_str(), hash.size(), SQLITE_STATIC);
		rc = sqlite3_bind_blob(stm, 4, last.c_str(), last.size(), SQLITE_STATIC);
		rc = sqlite3_bind_blob(stm, 5, lastfile.c_str(), lastfile.size()*sizeof(TCHAR), SQLITE_STATIC);


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

void DatabaseClass::SendVersions(SOCKET client, char* passw){
	int rc;
	/* Create SQL statement */
	std::string sql = "SELECT VER,LAST FROM VERSIONS GROUP BY VER,LAST";
	sqlite3_stmt* stm;
	//serialize list of files
	std::stringstream c;
	std::stringstream b;
	try{

		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_step(stm);

		std::string last_mod;
		int ver;
		int n = 0;
		if (rc != 100){
			wcout << L"NESSUN FILE PRESENTE NELLA CONFIGURAZIONE \n" << endl;
		}
		while (rc == 100){

			ver = sqlite3_column_int(stm, 0);
			last_mod = std::string((char*)sqlite3_column_blob(stm, 1), sqlite3_column_bytes(stm, 1));

			c.write(to_string(ver).c_str(), to_string(ver).size());
			c.write("\n", 1);
			b.write(last_mod.c_str(), last_mod.size());
			b.write("\n", 1);

			rc = sqlite3_step(stm);
			n++;
		}



		printf("I send the number of files in the last config\n");
		sendInt(client, n, passw);

		printf("I send the size of first file and the file\n");
		sendInt(client, c.str().size(), passw);
		invFile(client, (char*)c.str().c_str(), c.str().size(), passw);

		printf("I send the size of second file and the file\n");
		sendInt(client, b.str().size(), passw);
		invFile(client, (char*)b.str().c_str(), b.str().size(), passw);

	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during sending all version";
	}

	rc = sqlite3_finalize(stm);
	sendInt(client, -70, passw);
	return;
}

void DatabaseClass::eliminaFILE(wstring wpath, int ver){
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

int DatabaseClass::file_cancellati(int val){

	int versione = GetUltimaVersione();
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

void DatabaseClass::Register(string pass){
	sqlite3_stmt* stm = NULL;
	try{
		int  rc;
		std::string sql = "INSERT INTO CREDENTIAL (USER,PASS) VALUES (?1, ?2);";

		rc = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stm, NULL);
		rc = sqlite3_bind_text(stm, 1, nome.c_str(), nome.size(), SQLITE_STATIC);
		rc = sqlite3_bind_text(stm, 2, pass.c_str(), pass.size(), SQLITE_STATIC);
		rc = sqlite3_step(stm);

	}
	catch (...){
		sqlite3_finalize(stm);
		throw "error during registration";
	}
	sqlite3_finalize(stm);
}

int DatabaseClass::GetUltimaVersione(){
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

void DatabaseClass::PulisciDB(){

	int rc;
	int Versione = this->GetUltimaVersione();
	std::string sql = "DELETE FROM VERSIONS WHERE VER<?1";
	sqlite3_stmt* stm = NULL;
	sqlite3_stmt* stm2 = NULL;
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
			this->eliminaFILE(wpath, ver);

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

DatabaseClass::~DatabaseClass()
{
	int rc=sqlite3_close(db);
}
