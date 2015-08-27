#include "stdafx.h"
#include "Oggetto.h"
#include <string>
#include "HashFiles.h"


Oggetto::Oggetto(std::wstring path, std::wstring name)
{
	std::wstring pathcompleto;
	this->name = name;
	this->path = path;
	this->hash=ComputeHash(pathcompleto);
	backup = false;
	version = 1;
}


Oggetto::~Oggetto()
{
}
