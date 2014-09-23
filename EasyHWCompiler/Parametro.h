#include <iostream>
#include <string>
using namespace std;

class Parametro
{

private:
	wstring nombre;
	int tipo;
public:
	Parametro(wstring n, int t)
	{
		nombre = n;
		tipo = t;
	}
	wstring getNombre()
	{
		return nombre;
	}
	void setNombre(int n)
	{
		nombre = n;
	}
	int getTipo()
	{
		return tipo;
	}
	void setTipo(int t)
	{
		tipo = t;
	}
};