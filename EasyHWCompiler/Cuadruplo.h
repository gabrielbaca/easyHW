#include <list>
#include <iostream>
#include <string>
#include <stack>
using namespace std;
class Cuadruplo
{
public:
	Cuadruplo(wstring operador, wstring op1, wstring op2, wstring destino)
	{ 
		oper = operador; 
		opdo1 = op1; 
		opdo2 = op2; 
		dest= destino;
	}
	void setOp1(wstring op1)
	{
		opdo1 = op1;
	}
	void setOp2(wstring op2) 
	{
		opdo2 = op2;
	}
	void setOper(wstring operador) 
	{
		oper = operador;
	}
	void setDestino(wstring destino) 
	{
		dest = destino;
	}
	wstring getOp1() 
	{
		return opdo1;
	}
	wstring getOp2() 
	{
		return opdo2;
	}
	wstring getOper() 
	{
		return oper;
	}
	wstring getDestino() 
	{
		return dest;
	}
	void muestraCuadruplo()
	{
		wcout << oper << L"," << opdo1 << L"," << opdo2 << "," << dest << endl;
	}
private:
	wstring opdo1;
	wstring opdo2;
	wstring oper;
	wstring dest;
};