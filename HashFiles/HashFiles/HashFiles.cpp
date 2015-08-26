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
	//std::ofstream ofs("C:\\Users\\Paolo\\Desktop\\PROVA2\\ARRR.jpg", std::ios::binary);
	std::ostringstream STRINGAENORME;
	STRINGAENORME << ifs.rdbuf();
	string stringa(STRINGAENORME.str());
	string output1 = sha256(stringa);
	//ofs << stringa;
	//cout << "sha256('" << STRINGAENORME.str().length()<< "'):" << output1 << endl;

	ifs.close();
	//ofs.close();
	system("pause");
	return 0;

}

