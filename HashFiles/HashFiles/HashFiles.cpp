// HashFiles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sha.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{

	std::ifstream ifs("C:\\Users\\Paolo\\Desktop\\PROVA2\\Hydrangeas.jpg",std::ios::binary);
	std::ostringstream sstringa;
	sstringa << ifs.rdbuf();
	string stringa(sstringa.str());
	string output1 = sha256(stringa);
	ifs.close();
	
	
	//This part has the only pourpose to check that the string is the right one.
	//std::ofstream ofs("C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", std::ios::binary);
	//ofs << stringa;
	//cout << "sha256('" << STRINGAENORME.str().length()<< "'):" << output1 << endl;
	//ofs.close();
	
	return 0;

}

