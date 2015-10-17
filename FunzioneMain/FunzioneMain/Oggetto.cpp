#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "Sha.h"
#include <iostream>
#include "Windows.h"


using namespace std;

Oggetto::Oggetto(std::wstring path, std::wstring name, wstring LastModified, DWORD size,HANDLE nhandle)
{
	this->name = name;
	this->path = path;
	this->size = size;
	this->handle = nhandle;
	this->LastModified = LastModified;
	this->hash = sha256(path);
}
Oggetto::Oggetto(std::wstring path, std::wstring name, wstring LastModified,string hashpassed, DWORD size, HANDLE nhandle)
{
	this->name = name;
	this->path = path;
	this->size = size;
	this->handle = nhandle;
	this->LastModified = LastModified;
	this->hash = hashpassed;
}

Oggetto::Oggetto(Oggetto*old){
	this->name = old->GetName();
	this->path = old->GetPath();
	this->hash = old->GetHash();
	this->size = old->GetSize();
	this->handle = old->GetHandle();
	this->LastModified = old->GetLastModified();
}

Oggetto::~Oggetto()
{
	if (this->handle != INVALID_HANDLE_VALUE){
		if (!CloseHandle(this->handle)){
			std::cout << "HO CHIUSO UNA HANDLE MALE" << endl;
			std::cout << GetLastError()<<endl;
		}
		this->handle = INVALID_HANDLE_VALUE;
	}

}

