#pragma once
#include <string>
class Oggetto
{
private:
	std::wstring hash;
	std::wstring path;
	std::wstring name;
	bool backup;
	int version;
public:
	Oggetto(std::wstring path,std::wstring name);
	~Oggetto();
	//void setPath(); Forse inutile, magari potrebbe servire nel caso un oggetto venga spostato e vogliamo cambiare solo il path
};

