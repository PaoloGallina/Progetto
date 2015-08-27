#pragma once
#include <string>
class Oggetto
{
private:
	std::wstring name;
	std::wstring path;
	std::string hash;

public:
	Oggetto(std::wstring path,std::wstring name);
	~Oggetto();
};

