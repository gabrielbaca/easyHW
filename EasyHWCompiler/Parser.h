

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <fstream>
#include <direct.h>
#include "Variable.h"
#include "Componente.h"
#include "Cuadruplo.h"
#include "Parametro.h"
using namespace std;

//-------------------------------------------------------------------Cubo Semantico y Funciones-----------------------------------------------------------------------------

const int cubosem[13][5][5] = 
{
{{1, 2, 13, 13, 13}, {2, 2, 13, 13, 13}, {13, 13, 3/*Checar*/, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //+
{{1, 2, 13, 13, 13}, {2, 2, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //-
{{1, 2, 13, 13, 13}, {2, 2, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //*
{{2, 2, 13, 13, 13}, {2, 2, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, // /
{{1, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //AND 
{{1, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //OR 
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //<
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //>
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //!=
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}},//=
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}}, //<=
{{1, 1, 13, 13, 13}, {1, 1, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}},//>=
{{1, 1, 13, 13, 13}, {2, 2, 13, 13, 13}, {3, 3, 3, 13, 13}, {13, 13, 13, 13, 13}, {13, 13, 13, 13, 13}} // <-
};


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ID=1,
		_ENTERO=2,
		_DECIMAL=3,
		_FRASE=4,
		_LETRA=5,
		_ENDL=6,
		_HMOTOR=7,
		_HSENSOR=8,
		_HLED=9,
		_HPANTALLA=10,
		_HSERVO=11,
		_HBOTON=12,
		_PINANALOG=13
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

typedef map <wstring,void*> tablaDataFunc;
typedef map <int, Variable*> tablaVars;
typedef map <int, tablaDataFunc*> tablaProcs;
typedef map <int, Componente*> tablaHardware;
typedef list<Parametro*> Parametros;


int buscaCubo(wstring op, int opdo1, int opdo2)
{
  int oper;
  int aprob;
  
  if(op == L"+")
  {
	oper = 1;
  }
  else if (op == L"-")
  {
	oper = 2;
  }
  else if (op == L"*")
  {
	oper = 3;
  }
  else if (op == L"/")
  {
	oper = 4;
  }
  else if (op == L"Y")
  {
	oper = 5;
  }
  else if (op == L"O")
  {
	oper = 6;
  }
  else if (op == L"<")
  {
	oper = 7;
  }
  else if (op == L">")
  {
	oper = 8;
  }
  else if (op == L"!=")
  {
	oper = 9;
  }
  else if (op == L"=")
  {
	oper = 10;
  }
  else if (op == L"<=")
  {
	oper = 11;
  }
  else if (op == L">=")
  {
	oper = 12;
  }
  else if (op == L"<-")
  {
	oper = 13;
  } 

   aprob = cubosem[oper-1][opdo1-1][opdo2-1];
   return aprob;
}

void error()
{
	cout << "-----------------------ERRORES DE COMPILACION---------------------------" << endl;
	_getch();
	exit(1);	
}

wstring traduceTipoReal(int type)
{
	wstring tipoReal;
	switch(type)
	{
		case 1:
		case 4:
		case 5:
			tipoReal = L"int";
			break;
		case 6:
			tipoReal = L"double";
			break;
	}
	
	return tipoReal;
}

wstring traduceTipoParametro(int type)
{
	wstring tipoP;
	
	switch(type)
	{
		case 1:
			tipoP = L"int";
			break;
		case 2:
			tipoP = L"double";
			break;
		case 3:
			tipoP = L"string";
			break;
	}
	
	return tipoP;
}


//-----------------------------------------------------Declaracion de Variables Globales para el Programa-------------------------------------------------------------------
int myKey, procKey, varKey, hwKey, contRepeat, HWtype, VARtype;
bool pantallaOcupada;
wstring nombreProc, nombreVariable, nombreComponente;
wstring lastToken, nextToken, varScope;
wstring accionHWoPos;
wstring nombreFuncLl;
hash <wstring> wstr_hash;
tablaVars *ptrTVars;
tablaProcs *ptrTProcs;
tablaHardware *ptrTHW;
tablaDataFunc *ptrTDataF;
Variable *ptrVariable;
Componente *ptrComponente;
Parametros *ptrParametros;
list<Parametro*>::iterator itParams;
map <int, void*>::iterator it;
map <int, void*> tablaScope;
map <int,list<wstring>*> tLlParametros;
list<wstring>* ptrListaParametros;
int contLlFunc;
set <wstring> globalPinSet;
int direccion, revTipo;
int arrCasillas,dimension;
map <int,Cuadruplo*> tCuadruplos;
map <int,Cuadruplo*>::iterator iteC;
stack <wstring> POperadores, POperandos;
stack <int> PTipos; 
stack<int>PSaltos;
wstring operador, operando1, operando2, destino;
wstring idt; // ID temporal
wstring signoEntero;
// Variables 
int operandoT1, operandoT2;
// Temporal para el estatuto "while"
int auxTipo;
// Variables para crear temporales
int contT;
wstring strTemp;
// Variables para estatuto "if"
int contCuad;
int saltoFalso;
int saltoSino;
// Variables para estatutos ciclicos
int finM;
int finH;
int tipoVeces;
wstring varVeces;
//Variables para Expresiones
int tipoID;
int destinoT;
int tipoConstante;
bool declaredFunc;
bool llamandoFunc;
int contParams;
int tipoParametro;
int tipoArgumento;
int typeHW;
int tipoHW;
bool esTipoHW;
bool vinoPos;
int pinCont;
wstring pinesComponente;
//Variables utilizadas para la maquina virtual
string dirPath;
wstring pina, pinb, campoa, campob;
int find1, find2;
wstring opdoVM1, opdoVM2, destinationVM, operVM, tempVM;
list<wstring> *ptrParam;
wstring tipoPaux, nombrePaux;
wstring auxOperVM;
bool hayFunciones,vinoEnt,arregloTipoID,vienenParametros,encontreParam;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void EASYHW();
	void CONFIGURACION();
	void VARIABLES();
	void FUNC();
	void BLOQUE();
	void COMPONENTE();
	void PUERTOS();
	void NUMPIN();
	void DECLARACIONES();
	void SIMPLE();
	void ARREGLO();
	void TIPO();
	void TIPOA();
	void PARAMETROS();
	void FUNCVARS();
	void ESTATUTOS();
	void CONDICION();
	void CICLOS();
	void LLFUNC();
	void ASIGN();
	void EXPRESION();
	void MIENTRAS();
	void HAZ();
	void VECES();
	void LLPARAMETROS();
	void VARHW();
	void ASIGNACION();
	void EXP();
	void TERMINO();
	void FACTOR();
	void CTE();

	void Parse();

}; // end Parser



#endif

