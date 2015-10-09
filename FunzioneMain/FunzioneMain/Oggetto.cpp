#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "Sha.h"
#include "Windows.h"


using namespace std;
Oggetto::Oggetto(std::wstring path, std::wstring name, wstring LastModified,DWORD size)
{
	this->name = name;
	this->path = path;
	this->size = size;
	this->LastModified = LastModified;
	this->hash=sha256(path);
}

Oggetto::Oggetto(Oggetto*old){
	this->name = old->GetName();
	this->path = old->GetPath();
	this->hash = old->GetHash();
	this->size = old->GetSize();
	this->LastModified = old->GetLastModified();
}

Oggetto::~Oggetto()
{
}

