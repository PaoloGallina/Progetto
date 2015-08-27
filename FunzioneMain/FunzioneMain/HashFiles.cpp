

#include "stdafx.h"
#include "Sha.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;


wstring wComputeHash(std::wstring pathcompleto)
{
	std::ifstream ifs(pathcompleto,std::ios::binary);
	std::ostringstream sstringa;
	sstringa << ifs.rdbuf();
	ifs.close();
	string stringa(sstringa.str());
	string hash = sha256(stringa);
	wstring ret;
	sstringa.str("");//needed to clean it
	return ret.assign(hash.begin(),hash.end());
}


string ComputeHash(std::wstring pathcompleto)
{
	std::ifstream ifs(pathcompleto, std::ios::binary);
	std::ostringstream sstringa;
	sstringa << ifs.rdbuf();
	ifs.close();
	string stringa(sstringa.str());
	string hash = sha256(stringa);
	sstringa.str("");//needed to clean it
	return hash;
}
