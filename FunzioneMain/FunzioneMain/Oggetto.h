#pragma once
#include <windows.h>
#include <string>
class Oggetto
{
private:
	std::wstring name;
	std::wstring path;
	std::string hash;
	HANDLE handle=INVALID_HANDLE_VALUE;
	DWORD size;
	std::wstring LastModified;

public:
	Oggetto(std::wstring path, std::wstring name, std::wstring , DWORD size,HANDLE handle);
	Oggetto(std::wstring path, std::wstring name, std::wstring,std::string hashpassed, DWORD size, HANDLE handle);
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
	
	HANDLE GetHandle(){
		return this->handle;
	}
	
	void SetHandle(HANDLE handle){
		this->handle = handle;
	}

	~Oggetto();
};

