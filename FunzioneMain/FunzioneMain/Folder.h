
#include <string>
#include <iostream>
#include <list>
#include "fstream"
#include <future>
#include "Oggetto.h"
#pragma once
using namespace std;


class Folder
{
private:
	Folder& operator=(const Folder& source);  //operatore di assegnazione  DEVONO ESSERE COERENTI!!  DEVI ACCERTARTI DI NON ESSERE UGUALE A TE STESSO!
	Folder(const Folder& source);   //costruttore di copia
	std::list <Folder*> contains;

public:
	std::wstring name;
	Folder(std::wstring*, std::list <Oggetto*>&);
	~Folder();
};

