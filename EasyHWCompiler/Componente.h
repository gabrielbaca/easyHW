#include <iostream>
#include <string>
using namespace std;

class Componente
{

private:
	wstring nombre;
	int tipo;
	wstring pin;
	int tipoHW;

public:
	Componente()
	{
		nombre = L"";
		pin = L"";
		tipo = 0;
	}
	int getTipo(wstring opcion)
	{
		if(opcion == L".lectura" || opcion == L".angulo")
		{
			tipo = 2;
		}
		else if(opcion == L".imprime")
		{
			tipo = 3;
		}
		else
		{
			tipo = 1;
		}
		return tipo;
	}
	void setNombre(wstring n)
	{
		nombre = n;
	}
	wstring getNombre()
	{
		return nombre;
	}
	void setPin(wstring p)
	{
		pin = p;
	}
	wstring getPin()
	{
		return pin;
	}
	void setTipoHW(int tHW)
	{
		tipoHW = tHW;
	}
	int getTipoHW()
	{
		return tipoHW;
	}
};