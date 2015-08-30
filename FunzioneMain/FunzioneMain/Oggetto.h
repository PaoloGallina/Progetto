#pragma once
#include <string>
class Oggetto
{
private:
	std::wstring name;
	std::wstring path;
	std::string hash;
	std::wstring LastModified;

public:
	Oggetto(std::wstring path,std::wstring name,std::wstring LastModified);
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
	std::wstring GetLastModified(){
		return this->LastModified;
	}

	~Oggetto();
};

