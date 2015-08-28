#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "HashFiles.h"


Oggetto::Oggetto(std::wstring path, std::wstring name)
{
	this->name = name;
	this->path = path;
	this->hash=ComputeHash(path);
}


Oggetto::~Oggetto()
{
}
