
#include <string>
#include <iostream>
#include <list>
#include "fstream"
#include <future>
#include "Oggetto.h"
#pragma once
using namespace std;



class Cartella
{
private:
	Cartella& operator=(const Cartella& source);  //operatore di assegnazione  DEVONO ESSERE COERENTI!!  DEVI ACCERTARTI DI NON ESSERE UGUALE A TE STESSO!
	Cartella(const Cartella& source);   //costruttore di copia
	std::list <Cartella*> contains;

public:
	std::wstring name;
	Cartella(std::wstring*, std::list <Oggetto*>&, HANDLE hpipe,int flag);

	~Cartella();
};

