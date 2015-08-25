#include <string>
#include <iostream>
#include <list>
#include <string>
#include "fstream"
#include <future>
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
	static list<shared_future<wstring>> lista_promesse;
	Folder(std::wstring*, std::wstring*, std::wofstream&,std::wstring);
	~Folder();
	
};

