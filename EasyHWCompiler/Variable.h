#include <iostream>
using namespace std;
class Variable
{
private:
	wstring nombre;
	int tipo;
	int numCasillas;
public:
	Variable()
	{
		nombre = L"-";
		tipo = 0;
		numCasillas = 1;
	}
	Variable(wstring n, int t,int nC)
	{
		nombre = n;
		tipo = t;
		numCasillas = nC;
	}
	void setNombre(wstring name)
	{
		this->nombre = name;
	}
	wstring getNombre()
	{
		return nombre;
	}
	void setTipo(int type)
	{
		this->tipo = type;
	}
	int getTipo()
	{
		return tipo;
	}
	void setNumCasillas(int nC)
	{
		numCasillas = nC;
	}
	int getNumCasillas()
	{
		return numCasillas;
	}
};