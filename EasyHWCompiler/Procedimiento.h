#include <iostream>
using namespace std;

class Procedimiento
{

public:
	Procedimiento()
	{
		nameProc = L"-";
		ptrSet = NULL;
	}
	void setNameProc(wstring name)
	{
		nameProc = name;
	}
	void setPtrSet(set<Variable>* ptr)
	{
		ptrSet = ptr;
	}
	wstring getNameProc()
	{
		return nameProc;
	}
	set<Variable>* getPtrSet()
	{
		return ptrSet;
	}

private:
	wstring nameProc;
	set<Variable> *ptrSet; 

};