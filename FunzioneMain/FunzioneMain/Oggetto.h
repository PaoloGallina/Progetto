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
	Oggetto(Oggetto*);

	std::string GetHash(){
		return this->hash;
	}
	std::wstring GetPath(){
		return this->path;
	}
	std::wstring GetName(){
		return this->name;
	}

	~Oggetto();
};

