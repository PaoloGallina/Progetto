#include <Windows.h>
#pragma once
#include <string>
class Oggetto
{
private:
	std::wstring name;
	std::wstring path;
	std::string hash;
	DWORD size;
	std::wstring LastModified;

public:
	Oggetto(std::wstring path,std::wstring name,std::wstring LastModified,DWORD size);
	Oggetto(std::wstring path, std::wstring name, std::wstring LastModified,std::string hash, DWORD size);
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
	DWORD GetSize(){
		return this->size;
	}

	~Oggetto();
};

