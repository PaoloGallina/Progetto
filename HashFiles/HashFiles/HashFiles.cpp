

#include "stdafx.h"
#include "Sha.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


string ComputeHash()
{
	std::ifstream ifs("C:\\Users\\Paolo\\Desktop\\PROVA2\\Hydrangeas.jpg",std::ios::binary);
	std::ostringstream sstringa;
	sstringa << ifs.rdbuf();
	ifs.close();
	string stringa(sstringa.str());

	return sha256(stringa);
}

