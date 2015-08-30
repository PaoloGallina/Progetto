#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "HashFiles.h"

using namespace std;
Oggetto::Oggetto(std::wstring path, std::wstring name, wstring LastModified)
{
	this->name = name;
	this->path = path;
	this->LastModified = LastModified;
	this->hash = ComputeHash(path);
}

Oggetto::Oggetto(Oggetto*old){
	this->name = old->GetName();
	this->path = old->GetPath();
	this->hash = old->GetHash();
	this->LastModified = old->GetLastModified();
}

Oggetto::~Oggetto()
{
}

