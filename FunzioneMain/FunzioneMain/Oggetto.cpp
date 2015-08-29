#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "HashFiles.h"

using namespace std;
Oggetto::Oggetto(std::wstring path, std::wstring name)
{
	this->name = name;
	this->path = path;
	this->hash=ComputeHash(path);
}

Oggetto::Oggetto(Oggetto*old){
	this->name = old->GetName();
	this->path = old->GetPath();
	this->hash = old->GetHash();
}

Oggetto::~Oggetto()
{
}

