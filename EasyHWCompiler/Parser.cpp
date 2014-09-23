

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::EASYHW() {
		contCuad = 1; //Contador de cuadruplos
		llamandoFunc = false;
		esTipoHW = false;
		pinCont = 0;
		contT = 1;
		vinoPos = false;
		vinoEnt = false;
		arregloTipoID = false;
		vienenParametros = false;
		encontreParam = false;
		hayFunciones = false;
		contParams = 0;
		contLlFunc = 1;
		pinesComponente = L"";
		strTemp =L"_t";
		//Seccion para dar de alta los campos Hardware, Global y Local en la tabla de Scopes
		varScope = L"Hardware";
		myKey = wstr_hash(varScope);
		tablaScope.insert(pair <int, void*> (myKey, new tablaHardware));
		varScope = L"Global";
		myKey = wstr_hash(varScope);
		tablaScope.insert(pair <int, void*> (myKey, new tablaVars));
		varScope = L"Local";
		myKey = wstr_hash(varScope);
		tablaScope.insert(pair <int, void*> (myKey, new tablaProcs));
		//Rellenado del setGlobal de pines para incluir los pines estaticos de la pantalla
		pantallaOcupada = false;			
		signoEntero = L"";
		
		Expect(14 /* "CONFIG" */);
		varScope = L"Hardware";
		Expect(_ENDL);
		Expect(15 /* "{" */);
		Expect(_ENDL);
		CONFIGURACION();
		Expect(16 /* "}" */);
		while (la->kind == _ENDL) {
			Get();
		}
		Expect(17 /* "VARG" */);
		varScope = L"Global";
		Expect(_ENDL);
		Expect(15 /* "{" */);
		Expect(_ENDL);
		if (StartOf(1)) {
			VARIABLES();
		}
		Expect(16 /* "}" */);
		while (la->kind == _ENDL) {
			Get();
		}
		Expect(18 /* "FUNCIONES" */);
		varScope = L"Local";
		Expect(_ENDL);
		Expect(15 /* "{" */);
		Expect(_ENDL);
		if (la->kind == _ID) {
			FUNC();
			hayFunciones = true;
		}
		Expect(16 /* "}" */);
		llamandoFunc = true;
		while (la->kind == _ENDL) {
			Get();
		}
		Expect(19 /* "PROGRAMA" */);
		varScope = L"main";
		BLOQUE();
		dirPath = "C:\\Users\\SULO\\Documents\\Arduino\\";
		wofstream archIno;
		wofstream archDef;
		wofstream archLoop;
		wifstream archInDef,archInLoop;
		string projName;
		string pathCarpeta,pathArchivo,runArch;
		int tipoGlobalVar,tipoLocalVar;
		int tipoComponente;
		int posEspacioNew,posEspacioOld;
		int typePar;
		int contListParam;
		wstring pinesMotor;
		wstring funcName;
		wstring tipoReal;
		
		//Creacion y apertura de los archivos que contendran las partes de la traduccion
		archDef.open("Def.txt");
		archLoop.open("Loop.txt");
		
		//''''''''''''''''''''''''''''''''''''''''''Seccion de Hardware, Variables y Funciones'''''''''''''''''''''''''''''''''''''''''''''
		//Includes para elementos posibles de Hardware
		archLoop << L"#include <Servo.h>" << endl;
		archLoop << L"#include <LiquidCrystal.h>" << endl;
		archLoop << L"#include <String.h>" << endl;
		//Declaracion de variables de Hardware
		/*
		Tipos de Hardware
		-Motor 1
		-Servo 2
		-Pantalla 3
		-LED 4
		-Boton 5
		-Sensor 6			
		
		HMOTOR = "." ("velocidad"|"direccion").
		HSENSOR = "." "lectura".
		HLED = "." "encendido".
		HPANTALLA = "." "imprime".
		HSERVO = "." "angulo".
		HBOTON = "." "estado".						
		*/
		//cout << "------------------------Seccion de Hardware--------------------------------"<< endl;
		ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
		for(tablaHardware::iterator it = ptrTHW->begin(); it != ptrTHW->end(); it++)
		{
		tipoComponente = it->second->getTipoHW();
		//cout << tipoComponente << endl;
		switch(tipoComponente)
		{
			case 1://MOTOR
				pinesMotor = it->second->getPin();
				pinesMotor.erase(pinesMotor.begin());
				archLoop << traduceTipoReal(tipoComponente) << L" " << it->second->getNombre() << "=" << pinesMotor.substr(0,pinesMotor.find(L" "))<< L";" << endl;
				pinesMotor.erase(0,pinesMotor.find(L" ")+1);
				archLoop << L"int " << it->second->getNombre() + L"_fwd =" << pinesMotor.substr(0,pinesMotor.find(L" ")) << L";" << endl;
				pinesMotor.erase(0,pinesMotor.find(L" ")+1);
				archLoop << L"int " << it->second->getNombre() + L"_bwd =" << pinesMotor.substr(0,pinesMotor.length()) << L";" << endl;
				archLoop << L"int " << it->second->getNombre() + L"velocidad;" << endl;
				archLoop << L"int " << it->second->getNombre() + L"direccion;" << endl;
				break;						
			case 2://SERVO
				archLoop << L"Servo " << it->second->getNombre() << L";" << endl;	
				archLoop << L"int " << it->second->getNombre() << L"angulo;" << endl;
				break;						
			case 3://PANTALLA
				archLoop << L"LiquidCrystal " << it->second->getNombre() << L"(27,26,25,24,23,22);" << endl;
				archLoop << L"String " << it->second->getNombre() << L"imprime;" << endl;
				break;
			case 4://LED
				archLoop << traduceTipoReal(tipoComponente) << L" " << it->second->getNombre() << L"=" << it->second->getPin() << L";" << endl;
				archLoop << "int " << it->second->getNombre() + L"encendido;" << endl;
				break;
			case 5://BOTON
				archLoop << traduceTipoReal(tipoComponente) << L" " << it->second->getNombre() << L"=" << it->second->getPin() << L";" << endl;
				archLoop << "int " << it->second->getNombre() + L"estado;" << endl;
				break;
			case 6://SENSOR
				archLoop << L"int " << it->second->getNombre() << L"=" << it->second->getPin() << L";" << endl;
				archLoop << traduceTipoReal(tipoComponente) << " " << it->second->getNombre() + L"lectura;" << endl;
				break;
		}				
		
		}
					
		//Variables Globales
		//cout << "----------------Seccion de Variables Globales-----------------------"<< endl;
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		for(tablaVars::iterator it = ptrTVars->begin();it != ptrTVars->end();it++)
		{
		tipoGlobalVar = it->second->getTipo();
		switch(tipoGlobalVar)
		{
			case 1:
				archLoop << L"int " << it->second->getNombre() << L";" << endl;
				break;
			case 2:
				archLoop << L"double " << it->second->getNombre() << L";" << endl;
				break;
			case 3:
				archLoop << L"String " << it->second->getNombre() << L";" << endl;
				break;
			case 4:
				archLoop << L"int " << it->second->getNombre() << L"[" << it->second->getNumCasillas() << L"];" << endl;
				break;												
			case 5:
				archLoop << L"double " << it->second->getNombre() << L"[" << it->second->getNumCasillas() << L"];" << endl;
				break;
		}
							
		}	
		archLoop << L"int var_escritura;" << endl;				
		//'''''''''''''''''''''''''''''''''''''''''''''Seccion de SETUP''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//wcout << "----------------------Seccion de SETUP----------------------" << endl;
		ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
		archLoop << L"void setup()" << endl << L"{" << endl;
		for(tablaHardware::iterator it = ptrTHW->begin(); it != ptrTHW->end(); it++)
		{
		tipoComponente = it->second->getTipoHW();
		//cout << tipoComponente << endl;
		switch(tipoComponente)
		{
			case 1://MOTOR
				archLoop << L"\tpinMode(" << it->second->getNombre()  << L",OUTPUT);" << endl;
				archLoop << L"\tpinMode(" << it->second->getNombre() << L"_fwd,OUTPUT);" << endl;
				archLoop << L"\tpinMode(" << it->second->getNombre() << L"_bwd,OUTPUT);"<< endl;
				break;						
			case 2://SERVO
				archLoop << L"\t" << it->second->getNombre() << L".attach(" << it->second->getPin() << L");" << endl;								
				break;						
			case 3://PANTALLA
				archLoop << L"\t" << it->second->getNombre() << L".begin(16,2);" << endl;	
				archLoop << L"\t" << it->second->getNombre() << L".setCursor(0,1);" << endl;	
				break;
			case 4://LED
				archLoop << L"\tpinMode(" << it->second->getNombre()  << L",OUTPUT);"<< endl;
				break;
			case 5://BOTON
				archLoop << L"\tpinMode(" << it->second->getNombre()  << L",INPUT);"<< endl;
				break;
			case 6://SENSOR
				break;
		}				
		
		}				
					
		archLoop << L"}" << endl;
		//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		
		//'''''''''''''''''''''''''''''''''''''''''''''''Seccion de LOOP'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		//Funciones
		ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
		contListParam = 0;
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		operVM = tCuadruplos.find(1)->second->getOper();
		if(operVM == L"IniciaFunc")
		{}
		else
		{
		archLoop << L"int limites(int num)" << endl; 
		archLoop << L"{" << endl;
		archLoop << L"\tif(num<0)"<< endl << L"\tnum = 0;" << endl; // para menor que 0
		archLoop << L"\telse if(num>255)"<< endl << L"\tnum = 255;" << endl; // para mayor que 255
		archLoop << L"\treturn(num);" << endl;
		archLoop << L"}" << endl;
		archLoop << L"void loop()" << endl << L"{" << endl;
		}
		for(int i = 1; i <= tCuadruplos.size(); i++)
		{
		operVM = tCuadruplos.find(i)->second->getOper();
		if(operVM == L"IniciaFunc")
		{
			funcName = tCuadruplos.find(i)->second->getDestino();
			archLoop << L"void " << funcName << L"(";
			ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(wstr_hash(funcName)) -> second;
			ptrParametros = (Parametros*) ptrTDataF->find(L"fParams")->second;
			contListParam = 0;
			for(Parametros::iterator itP=ptrParametros->begin();itP != ptrParametros->end();itP++)
			{
				contListParam++;
				typePar = (*itP)->getTipo();
				archLoop <<  traduceTipoParametro(typePar) << L" " << (*itP)->getNombre();
				if(contListParam != ptrParametros->size())
					archLoop<< L", ";	
			}
			archLoop << L")" << endl << L"{" << endl;
			ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
			for(tablaVars::iterator it = ptrTVars->begin();it != ptrTVars->end();it++)
			{
				tipoLocalVar = it->second->getTipo();
				switch(tipoLocalVar)
				{
					case 1:
						archLoop << L"\tint " << it->second->getNombre() << L";" << endl;
						break;
					case 2:
						archLoop << L"\tdouble " << it->second->getNombre() << L";" << endl;
						break;
					case 3:
						archLoop << L"\tstring " << it->second->getNombre() << L";" << endl;
						break;
					case 4:
						archLoop << L"\tint " << it->second->getNombre() << L"[" << it->second->getNumCasillas() << L"];" << endl;
						break;												
					case 5:
						archLoop << L"\tdouble " << it->second->getNombre() << L"[" << it->second->getNumCasillas() << L"];" << endl;
						break;
				}	
			}						
		}
		
		if(operVM == L"FinFunc")
		{
			archLoop << L"}" << endl;
			auxOperVM = tCuadruplos.find(i+1)->second->getOper(); 
			if(auxOperVM == L"IniciaFunc")
			{
			}
			else
			{
				archLoop << L"int limites(int num)" << endl; 
				archLoop << L"{" << endl;
				archLoop << L"\tif(num<0)"<< endl << L"\tnum = 0;" << endl; // para menor que 0
				archLoop << L"\telse if(num>255)"<< endl << L"\tnum = 255;" << endl; // para mayor que 255
				archLoop << L"\treturn(num);" << endl;
				archLoop << L"}" << endl;
				
				archLoop << L"void loop()" << endl << L"{" << endl;
			
			}
		}
		
		if(L"delay" == operVM)
		{
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			archLoop << L"delay(" << destinationVM << L");"<< endl; 
		
		}
		
		
		if(L"<-" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			find2 = destinationVM.find(L".");
			if(find2 != -1)
			{							
				pinb = destinationVM.substr(0,find2);
				campob = destinationVM.substr(find2 + 1);
				destinationVM = pinb + campob;
				if(campob == L"imprime")
					opdoVM1 = L"(String) " + opdoVM1; 
				archLoop << L"\t" << destinationVM << L" = " << opdoVM1 << L";" << endl;
				//----------------Para Analog Write de asignacion-------------------------------------
						if(campob == L"velocidad")
						{
							archLoop << L"\tvar_escritura = limites(" << destinationVM << ");" << endl;
							archLoop << L"\tanalogWrite(" << pinb << L",var_escritura" << ");" << endl;	
						}
						if(campob == L"direccion")
						{
							archLoop << L"\tvar_escritura = limites(" << destinationVM << ");" << endl;
							archLoop << L"\tif( var_escritura == 0)" << endl << "\t{" << endl;
							//---Accion---------
							archLoop << L"\tdigitalWrite(" << pinb + L"_fwd" << L",LOW);" << endl;
							archLoop << L"\tdigitalWrite(" << pinb + L"_bwd" << L",HIGH);" << endl;
							archLoop << "\t}" << endl;
							//------------------
							archLoop << "\telse" << endl << "\t{" << endl; //Cuando es mayor a cero
							//----Accion--------
							archLoop << L"\tdigitalWrite(" << pinb + L"_fwd" << L",HIGH);" << endl;
							archLoop << L"\tdigitalWrite(" << pinb + L"_bwd" << L",LOW);" << endl;
							//------------------		
							archLoop << "\t}" << endl;
						}
						if(campob == L"estado")
						{
							cout << "ERROR!!No puedes escribir en un pin de entrada" << endl;
							error();
						}
						if(campob == L"encendido")
						{
							archLoop << L"\tvar_escritura = limites(" << destinationVM << ");" << endl;
							archLoop << L"\tif(var_escritura== 0)" << endl; //Cuando es igual a cero
							//----Accion-----
							archLoop << L"digitalWrite(" << pinb << L",LOW);" << endl;
							//---------------
							archLoop << L"else" << endl; //Cuando es mayor a cero
							//----Accion------
							archLoop << L"digitalWrite(" << pinb << L",HIGH);" << endl;
							//----------------
						}
						if(campob == L"imprime")
						{
							archLoop << pinb << L".clear();" << endl;
							archLoop << pinb << L".print(" << pinb + campob << L");" << endl;
						}
						if(campob == L"angulo")
						{
							archLoop << L"\tvar_escritura = limites(" << destinationVM << ");" << endl;
							archLoop << L"\tif(var_escritura > 180)" << endl;
							archLoop << L"\tvar_escritura = 180;" << endl;
							archLoop << pinb << L"." << L"write(var_escritura);" << endl;
						}
						if(campob == L"lectura" )
						{
							cout << "No puedes escribir en un pin de entrada" << endl;
							error();
						}
				//-------------------------------------------------------------------
			}
			else
			{
				archLoop << L"\t" << destinationVM << L" = " << opdoVM1 << L";" << endl;
			}
		} 
		else if(L"<" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" < " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L">" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" > " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"<=" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" < " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L">=" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" >= " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"=" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" == " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"+" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" + " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"-" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" - " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"Y" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" && " + opdoVM2 + L")";
			archDef << tempVM << endl;
		}
		else if(L"O" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" || " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"*" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" * " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		else if(L"/" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			opdoVM2 = tCuadruplos.find(i)->second->getOp2();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			find1 = opdoVM1.find(L".");
			find2 = opdoVM2.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			if(find2 != -1)
			{
				pinb = opdoVM2.substr(0, find2);
				campob = opdoVM2.substr(find2 + 1);
				if(campob == L"velocidad" || campob == L"direccion" || campob == L"angulo" || campob == L"encendido" || campob == L"estado"
				|| campob == L"imprime" || campob == L"lectura")
				{
					if(campob == L"estado")
						opdoVM2 =  L"digitalRead(" + pinb + L")";
					else if(campob == L"lectura")
						opdoVM2 =  L"analogRead(" + pinb + L")";
					else 
						opdoVM2 = L"\t" + pinb + campob;
				}
			}
			tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" / " + opdoVM2 + L")";
			archDef << tempVM << endl;
		} 
		
		else if(L"falsoM" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			find1 = opdoVM1.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0, find1);
				campoa = opdoVM1.substr(find1 + 1);
				if(campoa == L"velocidad" || campoa == L"direccion" || campoa == L"angulo" || campoa == L"encendido" || campoa == L"estado"
				|| campoa == L"imprime" || campoa == L"lectura")
				{
					if(campoa == L"estado")
						opdoVM1 =  L"digitalRead(" + pina + L")";
					else if(campoa == L"lectura")
						opdoVM1 =  L"analogRead(" + pina + L")";
					else 
						opdoVM1 = L"\t" + pina + campoa;
				}
			}
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			tempVM = L"while(" + opdoVM1 + L")";
			archLoop << L"\t" << tempVM << endl << L"\t{" << endl;
		} 
		else if(L"regresaM" == operVM)
		{
			archLoop  <<  L"\t}" << endl;
		} 
		else if(L"inicioHAZ" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			tempVM = L"\tfor(int i = 0; i < " + opdoVM1 + L"; i++)";
			archLoop << tempVM << endl << L"\t{"<< endl;
		} 
		else if(L"finHAZ" == operVM)
		{
			archLoop << L"\t}" << endl;
		} 
		else if(L"inicioIf" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			find1 = opdoVM1.find(L".");
			if(find1 != -1)
			{
				pina = opdoVM1.substr(0,find1-1);
				campoa = opdoVM1.substr(find1+1);
				opdoVM1 = L"\t" + pina + campoa;
			}
			archLoop << L"\tif(" + opdoVM1 + L")" << endl;
			archLoop << L"\t{" << endl;
		}
		else if(L"finIf" == operVM)
		{
			archLoop << L"\t}" << endl;
		}
		else if(L"inicioElse" == operVM)
		{
			archLoop << L"\telse" << endl << L"\t{" << endl;
		}
		else if(L"finElse" == operVM)
		{
			archLoop << L"\t}" << endl;
		}
		else if(L"llamaFunc" == operVM)
		{
			opdoVM1 = tCuadruplos.find(i)->second->getOp1();
			destinationVM = tCuadruplos.find(i)->second->getDestino();
			archLoop << L"\t" <<destinationVM << L"(";
			if(opdoVM1 != L"0")
			{
				ptrParam = tLlParametros.find(stoi(opdoVM1))->second;
				//if(!(ptrParam->size() == 0))
				//{
					for(int f = 1; f < ptrParam->size(); f++)
					{
						ptrParam = tLlParametros.find(stoi(opdoVM1))->second;
						nombrePaux = ptrParam->front();
						ptrParam->pop_front();
						archLoop << nombrePaux << L",";
					}
					nombrePaux = ptrParam->front();
				//}
			}
			archLoop << nombrePaux << L");" << endl;
		}
		}
		archLoop << L"}";
		
		
		
		
		
		
		
		
		
		
		
		
		
		//wcout << L"}" << endl;
		//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''			
		
		cout << "El numero de cuadruplos generado es " << tCuadruplos.size() << endl;
		cout << "----------------------Cuadruplos-------------------" << endl;
		for(map<int,Cuadruplo*>::iterator it=tCuadruplos.begin();it!=tCuadruplos.end();++it)
		{ 	cout << it->first << " ";
		it->second->muestraCuadruplo();
		}
		
					
		
		//Cierre de Archivo y Ejecutar IDE de ARDUINO
		archDef.close();
		archLoop.close();
		cout << "COMPILACION EXITOSA!!" << endl;
		cout << "Teclea el nombre del proyecto (Recuerda que sera guardado en la carpeta de Arduino): ";
		cin >> projName;
		pathCarpeta = dirPath + projName;
		pathArchivo = pathCarpeta + "\\" + projName + ".ino";
		mkdir(pathCarpeta.c_str());
		wstring linea;
		//Juntar todos los archivos para crear el archivo INO
		
		archIno.open(pathArchivo);
		archInDef.open("Def.txt");
		archInLoop.open("Loop.txt");
		
		while(!archInDef.eof())
		{
		getline(archInDef,linea);
		archIno << linea << endl;				
		}
		
		archInDef.close();
		
		while(!archInLoop.eof())
		{
		getline(archInLoop,linea);
		archIno << linea << endl;
		
		}
		
		archInLoop.close();
									
		archIno.close();
		//Correr el IDE de Arduino				
		runArch = "start " + pathArchivo;
		system(runArch.c_str());
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		
		Expect(_EOF);
}

void Parser::CONFIGURACION() {
		COMPONENTE();
		Expect(_ID);
		nombreComponente = (wstring) t -> val;
		hwKey = wstr_hash(nombreComponente);
		ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(varScope)) -> second;
		contRepeat = ptrTHW -> count(hwKey);
		if(contRepeat == 0)
		{
		//Si el id no esta repetido se agrega a la tabla de hardware
		ptrTHW -> insert(pair <int, Componente*> (hwKey, new Componente));
		ptrComponente = (Componente*) ptrTHW -> find(hwKey) -> second;
		ptrComponente -> setNombre(nombreComponente);
		ptrComponente -> setTipoHW(typeHW);
		}
		else
		{
		//Si el id ya esta asignado se marca error de variable ya definida
		wcout << t->line << L":ERROR!! El componente " << nombreComponente << L" ya esta definido" << endl; 
		error();
		}
		
		if (la->kind == 27 /* "pin" */) {
			PUERTOS();
		}
		pinCont = 0;
		Expect(_ENDL);
		if (StartOf(2)) {
			CONFIGURACION();
		}
}

void Parser::VARIABLES() {
		DECLARACIONES();
		Expect(_ENDL);
		if (StartOf(1)) {
			VARIABLES();
		}
}

void Parser::FUNC() {
		Expect(_ID);
		nombreProc = (wstring)t -> val;
		procKey = wstr_hash(nombreProc);
		ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second; 		
		contRepeat = ptrTProcs -> count(procKey);
		if(contRepeat == 0)
		{
		declaredFunc = false;
		ptrTProcs -> insert(pair<int, tablaDataFunc*>(procKey, new tablaDataFunc));	
		ptrTDataF = (tablaDataFunc*)ptrTProcs->find(procKey)->second;
		ptrTDataF->insert(pair<wstring,void*>(L"fParams",new Parametros));
		ptrTDataF->insert(pair<wstring,void*>(L"fVars",new tablaVars));
		//cout << "SE AGREGO UNA NUEVA FUNCION A LA TABLA DE PROCEDIMIENTOS" << endl;
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"IniciaFunc",L"",L"",nombreProc)));
		contCuad++;
		}
		else
		{
		declaredFunc = true;
		wcout << t->line << L":ERROR!! La funcion " << nombreProc << L" ya se encuentra definida" << endl;
		error();
		}
		
		Expect(34 /* "(" */);
		if (la->kind == 29 /* "entero" */ || la->kind == 30 /* "decimal" */ || la->kind == 31 /* "frase" */) {
			PARAMETROS();
		}
		Expect(35 /* ")" */);
		Expect(_ENDL);
		if (la->kind == 36 /* "VARF" */) {
			FUNCVARS();
		}
		BLOQUE();
		if(!declaredFunc)
		{
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"FinFunc",L"",L"",nombreProc)));
		contCuad++;
		}
		
		if (la->kind == _ID) {
			FUNC();
		}
}

void Parser::BLOQUE() {
		Expect(_ENDL);
		Expect(15 /* "{" */);
		Expect(_ENDL);
		if (StartOf(3)) {
			ESTATUTOS();
		}
		Expect(16 /* "}" */);
		while (la->kind == _ENDL) {
			Get();
		}
}

void Parser::COMPONENTE() {
		switch (la->kind) {
		case 20 /* "motor" */: {
			Get();
			HWtype = 1; typeHW = 1;
			break;
		}
		case 21 /* "servo" */: {
			Get();
			HWtype = 1; typeHW = 2;
			break;
		}
		case 22 /* "pantalla" */: {
			Get();
			if(!pantallaOcupada)
			{
			HWtype = 3; 
			typeHW = 3;
			pantallaOcupada = true;
			//contRepeat = globalPinSet.count(L"24");
			if(globalPinSet.count(L"27") != 0 || globalPinSet.count(L"26") != 0 || globalPinSet.count(L"25") != 0 || globalPinSet.count(L"24") != 0 || globalPinSet.count(L"23") != 0 || globalPinSet.count(L"22") != 0)
			{
				cout << t->line << ":ERROR!! Recuerda que solo puedes utilizar una Pantalla, y que no puedes hacer uso de los pines 27, 26, 25, 24, 23, 22" << 
				" si es que planeas utilizar la Pantalla en tu robot" << endl;
				error();							
			}
			else
			{
				globalPinSet.insert(L"27");
				globalPinSet.insert(L"26");
				globalPinSet.insert(L"25");
				globalPinSet.insert(L"24");
				globalPinSet.insert(L"23");
				globalPinSet.insert(L"22");	
			}
			}
			else
			{
			cout << t->line << ":ERROR!! Recuerda que solo puedes utilizar una Pantalla, y que no puedes hacer uso de los pines 27, 26, 25 24, 23, 22" << 
			" si es que planeas utilizar la Pantalla en tu robot" << endl;
			error();
			}
			
			break;
		}
		case 23 /* "led" */: {
			Get();
			HWtype = 1; typeHW = 4; 
			break;
		}
		case 24 /* "boton" */: {
			Get();
			HWtype = 1; typeHW = 5;
			break;
		}
		case 25 /* "sensor" */: {
			Get();
			HWtype = 2; typeHW = 6;
			break;
		}
		default: SynErr(62); break;
		}
}

void Parser::PUERTOS() {
		Expect(27 /* "pin" */);
		NUMPIN();
		pinesComponente = L"";
		switch(typeHW)
		{
		case 1:
			if(pinCont != 3)
			{
				wcout << t->line << L":ERROR!! Un motor no puede tener ni mas ni menos de 3 pins" << endl;
				error();
			}
			break;	
		case 2:
			if(pinCont != 1)
			{
				wcout << t->line << L":ERROR!! Un Servo no puede tener mas de 1 pin" << endl;
				error();
			}
			break;
		case 4:
			if(pinCont != 1)
			{
				wcout << t->line << L":ERROR!! Un LED no puede tener mas de 1 pin" << endl;
				error();
			}
			break;
		case 5:
			if(pinCont != 1)
			{
				wcout << t->line << L":ERROR!! Un Boton no puede tener mas de 1 pin" << endl;
				error();
			}
			break;
		case 6:
			if(pinCont != 1)
			{
				wcout << t->line << L":ERROR!! Un Sensor no puede tener mas de 1 pin" << endl;
				error();
			}
			break;
		}				
						
		
}

void Parser::NUMPIN() {
		if (la->kind == _ENTERO) {
			Get();
		} else if (la->kind == _PINANALOG) {
			Get();
		} else SynErr(63);
		pinCont++;
		//Seccion para asignar los pines a cada componente de HW
		lastToken = (wstring) t -> val;
		contRepeat = globalPinSet.count(lastToken);
		if(contRepeat == 0)
		{
		globalPinSet.insert(lastToken);
		
		if(typeHW != 1)
		{
			ptrComponente -> setPin(lastToken);
		}
		else
		{
			ptrComponente -> setPin( pinesComponente + L" " + lastToken);
			pinesComponente = ptrComponente->getPin();
		}
		}
		else
		{
		wcout << t->line  <<L":ERROR!! El pin " << lastToken << L" ya esta siendo utilizado" << endl;
		error();
		}				
		
		if (la->kind == 26 /* "," */) {
			Get();
			NUMPIN();
		}
}

void Parser::DECLARACIONES() {
		if (la->kind == 29 /* "entero" */ || la->kind == 30 /* "decimal" */ || la->kind == 31 /* "frase" */) {
			SIMPLE();
		} else if (la->kind == 32 /* "enteroA" */ || la->kind == 33 /* "decimalA" */) {
			ARREGLO();
		} else SynErr(64);
}

void Parser::SIMPLE() {
		TIPO();
		Expect(_ID);
		nombreVariable = (wstring) t -> val;
		varKey = wstr_hash(nombreVariable);
		//-----------------------Si nos encontramos en la seccion de declaraciones GLOBALES se hace lo siguiente-------------------------------------------
		if(varScope == L"Global")
		{
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat == 0)
		{
			
			ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
			contRepeat = ptrTHW -> count(varKey);
			if(contRepeat == 0)
			{
				ptrTVars -> insert(pair <int, Variable*> (varKey, new Variable));
				ptrVariable = (Variable*) ptrTVars -> find(varKey) -> second;
				ptrVariable -> setNombre(nombreVariable);
				ptrVariable -> setTipo(VARtype);
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de HARDWARE" << endl;
				error();
			}					
		}
		else
		{
			wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de GLOBALES" << endl;
			error();
		}					
		}
		//-----------------------Si nos encontramos en la seccion de declaraciones LOCALES se hace lo siguiente---------------------------------------------
		else if(varScope == L"Local")
		{
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global"))->second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat == 0)
		{
			ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
			contRepeat = ptrTHW -> count(varKey);
			if(contRepeat == 0)
			{
				ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
				ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
				ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
				contRepeat = ptrTVars -> count(varKey);
				if(contRepeat == 0)
				{
					ptrTVars -> insert(pair <int, Variable*> (varKey, new Variable));
					ptrVariable = (Variable*) ptrTVars -> find(varKey) -> second;
					ptrVariable -> setNombre(nombreVariable);
					ptrVariable -> setTipo(VARtype);
				}
				else
				{
					wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya se encuentra definida en la funcion " << nombreProc << endl;
					error();
				}
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de HARDWARE" << endl;
				error();
			}					
		}
		else
		{
			wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de GLOBALES" << endl;
			error();
		}				
		}
		
}

void Parser::ARREGLO() {
		TIPOA();
		Expect(_ID);
		nombreVariable = (wstring)t -> val;
		
		Expect(28 /* "espacios" */);
		Expect(_ENTERO);
		dimension = stoi((wstring)t->val);
		varKey = wstr_hash(nombreVariable);
		//---------------Si nos encontramos en la seccion de declaraciones GLOBALES se hace lo siguiente---------------------------------------------
		if(varScope == L"Global")
		{
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat == 0)
		{
			ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
			contRepeat = ptrTHW -> count(varKey);
			if(contRepeat == 0)
			{
				ptrTVars -> insert(pair<int, Variable*>(varKey, new Variable));
				ptrVariable = (Variable*) ptrTVars -> find(varKey) -> second;
				ptrVariable -> setNombre(nombreVariable);
				ptrVariable -> setTipo(VARtype);
				ptrVariable -> setNumCasillas(dimension);
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de HARDWARE" << endl;
				error();
			}					
		}
		else
		{
			wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de GLOBALES" << endl;
			error();
		}					
		}
		//------------------------Si nos encontramos en la seccion de declaraciones LOCALES se hace lo siguiente--------------------------------------
		else if(varScope == L"Local")
		{
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat == 0)
		{
			ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
			contRepeat = ptrTHW -> count(varKey);
			if(contRepeat == 0)
			{
				ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
				ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
				ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
				contRepeat = ptrTVars -> count(varKey);
				if(contRepeat == 0)
				{
					ptrTVars -> insert(pair<int, Variable*>(varKey, new Variable));
					ptrVariable = (Variable*) ptrTVars -> find(varKey) -> second;
					ptrVariable -> setNombre(nombreVariable);
					ptrVariable -> setTipo(VARtype);
					ptrVariable -> setNumCasillas(dimension);
					//cout << "La dimension del arreglo es: " << dimension << endl;
				}
				else
				{
					wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya se encuentra definida en la funcion " << nombreProc << endl;
					error();
				}
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de HARDWARE" << endl;
				error();
			}					
		}
		else
		{
			wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de GLOBALES" << endl;
			error();
		}							
		}
		
		
}

void Parser::TIPO() {
		if (la->kind == 29 /* "entero" */) {
			Get();
			VARtype = 1;
		} else if (la->kind == 30 /* "decimal" */) {
			Get();
			VARtype = 2;
		} else if (la->kind == 31 /* "frase" */) {
			Get();
			VARtype = 3;
		} else SynErr(65);
}

void Parser::TIPOA() {
		if (la->kind == 32 /* "enteroA" */) {
			Get();
			VARtype = 4;
		} else if (la->kind == 33 /* "decimalA" */) {
			Get();
			VARtype = 5;
		} else SynErr(66);
}

void Parser::PARAMETROS() {
		TIPO();
		Expect(_ID);
		nombreVariable = (wstring) t -> val;
		varKey = wstr_hash(nombreVariable);
											
		//Cada parametro es agregado como una variable local a la funcion, y se verifica que no este definida 
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global"))->second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat == 0)
		{
		ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
		contRepeat = ptrTHW -> count(varKey);
		if(contRepeat == 0)
		{
			ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
			ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
			ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
			contRepeat = ptrTVars -> count(varKey);
			if(contRepeat == 0)
			{
				//ptrTVars -> insert(pair <int, Variable*> (varKey, new Variable));
				//ptrVariable = (Variable*) ptrTVars -> find(varKey) -> second;
				//ptrVariable -> setNombre(nombreVariable);
				//ptrVariable -> setTipo(VARtype);
			}
			else
			{
				wcout << t->line << L":ERROR!!La variable " << nombreVariable << L" ya se encuentra definida en la funcion " << nombreProc << endl;
				error();
			}
		}
		else
		{
			wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de HARDWARE" << endl;
			error();
		}					
		}
		else
		{
		wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" ya esta definida en la seccion de GLOBALES" << endl;
		error();
		}				
						
		//Seccion de Codigo Anterior para agregar parametros a la tabla de parametros de cada funcion
		ptrParametros = (Parametros*) ptrTDataF->find(L"fParams")->second;
		ptrParametros->push_back(new Parametro(nombreVariable,VARtype));
		for(list<Parametro*>::iterator it=ptrParametros->begin();it!=ptrParametros->end();++it)
		{
		//wcout << "Nombre de Parametro: " << (*it)->getNombre() << endl;
		//wcout << "Tipo de Parametro: " << (*it)->getTipo() << endl;
		}		
		
		
		
		if (la->kind == 26 /* "," */) {
			Get();
			PARAMETROS();
		}
}

void Parser::FUNCVARS() {
		Expect(36 /* "VARF" */);
		varScope = L"Local";
		Expect(_ENDL);
		Expect(37 /* "-" */);
		Expect(_ENDL);
		if (StartOf(1)) {
			VARIABLES();
		}
		Expect(37 /* "-" */);
}

void Parser::ESTATUTOS() {
		if (la->kind == 38 /* "condicion" */) {
			Get();
			CONDICION();
		} else if (la->kind == 39 /* "ciclo" */) {
			Get();
			CICLOS();
		} else if (la->kind == 40 /* "llamar" */) {
			Get();
			LLFUNC();
			Expect(_ENDL);
		} else if (la->kind == 41 /* "asignar" */) {
			Get();
			ASIGN();
			Expect(_ENDL);
		} else if (la->kind == 42 /* "esperar" */) {
			Get();
			Expect(_ENTERO);
			lastToken = (wstring)t->val;
			tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"delay", L"", L"", lastToken)));
			contCuad++;
			
			Expect(_ENDL);
		} else SynErr(67);
		if (StartOf(3)) {
			ESTATUTOS();
		}
}

void Parser::CONDICION() {
		Expect(43 /* "si" */);
		Expect(34 /* "(" */);
		EXPRESION();
		Expect(35 /* ")" */);
		if(PTipos.top() != 1)
		{
		PTipos.pop();
		wcout << t->line << L":ERROR!! Expresion en la linea " << t->line << L" tiene que ser entera" << endl;
		error();
		}
		else
		{
		operando1 = POperandos.top();
		//wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
		POperandos.pop();
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"inicioIf", operando1, L"", L"")));
		//wcout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		PSaltos.push(contCuad);
		contCuad++;
		
		}				
		
		BLOQUE();
		saltoFalso = PSaltos.top();
		PSaltos.pop();
		tCuadruplos.find(saltoFalso)->second->setDestino(to_wstring((long double)contCuad+1));
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"finIf", L"", L"", L"")));
		contCuad++;
		
		
		if (la->kind == 44 /* "sino" */) {
			Get();
			tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"inicioElse", L"", L"", L"")));
			//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
			PSaltos.push(contCuad);
			contCuad++;
			
			BLOQUE();
			saltoSino = PSaltos.top();
			PSaltos.pop();
			tCuadruplos.find(saltoSino)->second->setDestino(to_wstring((long double)contCuad+1));
			tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"finElse", L"", L"", L"")));
			contCuad++;
			
		}
}

void Parser::CICLOS() {
		if (la->kind == 45 /* "mientras" */) {
			MIENTRAS();
		} else if (la->kind == 46 /* "haz" */) {
			HAZ();
		} else SynErr(68);
}

void Parser::LLFUNC() {
		Expect(_ID);
		nombreFuncLl = (wstring)t->val;
		procKey = wstr_hash(nombreFuncLl);
		ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second; 		
		contRepeat = ptrTProcs -> count(procKey);
		if(contRepeat != 0)
		{
		//wcout << "Se esta llamando a " << nombreFuncLl << " y si esta definida" << endl;
		ptrTDataF = (tablaDataFunc*)ptrTProcs->find(procKey)->second;
		ptrParametros = (Parametros*) ptrTDataF->find(L"fParams")->second;
		itParams = ptrParametros->begin();					
		}
		else
		{
		wcout << t->line << ":ERROR!! La funcion " << nombreFuncLl << " no se encuentra declarada" << endl;
		error();
		}
		
		Expect(34 /* "(" */);
		if (la->kind == _ID) {
			ptrListaParametros = new list<wstring>;
			vienenParametros = true;
			
			LLPARAMETROS();
		}
		Expect(35 /* ")" */);
		if(contParams != ptrParametros->size())
		{
		cout << t->line << ":ERROR!! Existen menos argumentos que los que necesita la funcion" << endl;
		error();
		}
		if(vienenParametros)
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"llamaFunc", to_wstring((long double)contLlFunc), L"",nombreFuncLl)));
		else
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"llamaFunc", L"0" , L"",nombreFuncLl)));
		contCuad++;
		tLlParametros.insert(pair<int,list<wstring>*>(contLlFunc,ptrListaParametros));
		contLlFunc++; 
		contParams = 0;		
		vienenParametros = false;
		
}

void Parser::ASIGN() {
		Expect(_ID);
		nombreVariable = (wstring) t -> val;
		idt = nombreVariable;
		varKey = wstr_hash(nombreVariable);	
		
		if (StartOf(4)) {
			if (StartOf(5)) {
				VARHW();
			} else {
				Get();
				if (la->kind == _ENTERO) {
					Get();
					vinoEnt = true;
				} else if (la->kind == _ID) {
					Get();
				} else SynErr(69);
				vinoPos = true;
			}
		}
		accionHWoPos = (wstring) t->val;
		
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		contRepeat = ptrTVars -> count(varKey);
		if(contRepeat != 0)
		{
		ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
		tipoID = ptrVariable->getTipo();
		if(tipoID == 4 || tipoID == 5)
		{
			if(vinoPos)
			{
				//AQUI ESTA LO QUE SE CAMBIO PARA ARREGLOS----------------------
				if(!vinoEnt)
				{
					ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
					contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
					if(contRepeat != 0)
					{
						ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
						arregloTipoID = ptrVariable->getTipo();
					}
					else
					{
						if(varScope == L"Local")
						{
							ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
							ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
							ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
							contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
							if(contRepeat != 0)
							{
								ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
								arregloTipoID = ptrVariable->getTipo();
							}
							else
							{
								ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
								ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
								ptrTVars = (tablaVars*) ptrTDataF -> find (L"fParams") -> second;
								contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
								if(contRepeat != 0)
								{
									ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
									arregloTipoID = ptrVariable->getTipo();
								}
								else
								{
									wcout << t->line << L":ERROR!! La variable " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
									error();
								}
							}
						}
						else
						{
							wcout << t->line << L":ERROR!! La variable " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
							error();							
						}
					}
		
					if(arregloTipoID != 1)
					{
						cout << t->line << ":La variable utilizada para indexar en el arreglo no es de tipo entero!" << endl;
						error();
					}
				}
				else
				{
					arrCasillas = ptrVariable->getNumCasillas();
					if(arrCasillas < stoi(accionHWoPos) || stoi(accionHWoPos) < 1)
					{
						cout << t->line << ":ERROR!! Fuera de los limites del arreglo!!" << endl;
						error();
					}
				}
		
				nombreVariable = nombreVariable + L"[" + accionHWoPos + L"-1]";
				if(tipoID==4)
					tipoID = 1;
				else
					tipoID = 2;
			}
			else
			{
				wcout << t->line << ":ERROR!! Debe venir posicion del arreglo" << endl;
				error();
			}	
		}
		
		POperandos.push(nombreVariable);
		PTipos.push(tipoID);
		}
		else
		{
		ptrTHW = (tablaHardware*) tablaScope.find(wstr_hash(L"Hardware")) -> second;
		contRepeat = ptrTHW -> count(varKey);
		if(contRepeat != 0)
		{
			ptrComponente = (Componente*) ptrTHW->find(varKey)->second;
			tipoID = ptrComponente->getTipo(accionHWoPos);
			POperandos.push(nombreVariable + accionHWoPos);
			PTipos.push(tipoID);
		}
		else
		{
			if(varScope == L"Local")
			{
				ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
				ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
				ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
				contRepeat = ptrTVars -> count(varKey);
				if(contRepeat != 0)
				{
					ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
					tipoID = ptrVariable->getTipo();
					if(tipoID == 4 || tipoID == 5)
					{
						if(vinoPos)
						{
							//AQUI ESTA LO QUE SE CAMBIO PARA ARREGLOS----------------------
							if(!vinoEnt)
							{
								ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
								contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
								if(contRepeat != 0)
								{
									ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
									arregloTipoID = ptrVariable->getTipo();
								}
								else
								{	
									if(varScope == L"Local")
									{
										ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
										ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
										ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
										contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
										if(contRepeat != 0)
										{
											ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
											arregloTipoID = ptrVariable->getTipo();
										}
										else
										{
											ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
											ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
											ptrTVars = (tablaVars*) ptrTDataF -> find (L"fParams") -> second;
											contRepeat = ptrTVars -> count(wstr_hash(accionHWoPos));
											if(contRepeat != 0)
											{
												ptrVariable = (Variable*) ptrTVars->find(wstr_hash(accionHWoPos))->second;
												arregloTipoID = ptrVariable->getTipo();
											}
											else
											{												
												wcout << t->line << L":ERROR!! La variable " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
												error();
											}
										}
									}
									else
									{
										wcout << t->line << L":ERROR!! La variable " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
										error();
									}
								}
		
								if(arregloTipoID != 1)
								{	
		
									cout << t->line << L":ERROR!! La variable utilizada para indexar en el arreglo no es de tipo entero!" << endl;
									error();
								}
							}
							else
							{
								arrCasillas = ptrVariable->getNumCasillas();
								if(arrCasillas < stoi(accionHWoPos) || stoi(accionHWoPos) < 1)
								{
									cout << t->line << ":ERROR!! Fuera de los limites del arreglo!!" << endl;
									error();
								}
							}
		
							nombreVariable = nombreVariable + L"[" + accionHWoPos + L"-1]";
							if(tipoID==4)
								tipoID = 1;
							else
								tipoID = 2;
						}
						else
						{
							wcout << t->line << ":ERROR!! Debe venir posicion del arreglo" << endl;
							error();
						}	
					}
		
					POperandos.push(nombreVariable);
					PTipos.push(tipoID);
		
				}
				else
				{
					ptrParametros = (Parametros*) ptrTDataF->find(L"fParams")->second;
					for(Parametros::iterator itP=ptrParametros->begin();itP != ptrParametros->end();itP++)
					{
						if((*itP)->getNombre() == nombreVariable)
						{
							encontreParam = true;
							tipoID = (*itP)->getTipo();
						}
					}
					
					if(!encontreParam)
					{
						wcout << t->line << L":ERROR!! La variable " << nombreVariable << " no se encuentra definida" << endl;
						error();
					}
					else
					{
						POperandos.push(nombreVariable);
						PTipos.push(tipoID);
						encontreParam = false;
					}
				}
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable " << nombreVariable << " no se encuentra definida" << endl;
				error();
			}
		}	
		}
		
		vinoEnt = false;
		vinoPos = false;
		
		ASIGNACION();
}

void Parser::EXPRESION() {
		EXP();
		if(POperadores.size() != 0)
		{
		//wcout << "El top de POperadores en FACTOR es: " << POperadores.top();
		
		if(POperadores.top() == L"<" || POperadores.top() == L">" || POperadores.top() == L"<=" || POperadores.top() == L">=" || POperadores.top() == L"!=" || POperadores.top() == L"=" || POperadores.top() == L"Y" || POperadores.top() == L"O")
		{
			operador = POperadores.top();
			POperadores.pop();
			operandoT2 = PTipos.top();
			PTipos.pop();
			operandoT1 = PTipos.top();
			PTipos.pop();
			revTipo = buscaCubo(operador, operandoT1, operandoT2);
			if(revTipo == 13)
			{
				wcout << t->line << L":ERROR!! Tipos no compatibles en la linea " << t -> line << endl;
				error();
			} 
			else
			{
				operando2 = POperandos.top();
				//wcout << "Se esta a punto de sacar el operando2 EXPRESION " << POperandos.top() << endl;
				POperandos.pop();
				operando1 = POperandos.top();
				//wcout << "Se esta a punto de sacar el operando1 EXPRESION" << POperandos.top() << endl;
				POperandos.pop();
				strTemp = L"_t" + to_wstring((long double)contT);
				destino = strTemp;
			    tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(operador, operando1, operando2, destino)));//Genera cuadruplo
				//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
				//wcout << "Se esta a punto de meter el operando EXPRESION " << destino << endl;
			    POperandos.push(destino);
			    PTipos.push(revTipo);
				contCuad++;
			    contT++;
			}
		}
		}
		
		if (StartOf(6)) {
			switch (la->kind) {
			case 50 /* ">" */: {
				Get();
				break;
			}
			case 51 /* "<" */: {
				Get();
				break;
			}
			case 52 /* "<=" */: {
				Get();
				break;
			}
			case 53 /* ">=" */: {
				Get();
				break;
			}
			case 54 /* "!=" */: {
				Get();
				break;
			}
			case 55 /* "=" */: {
				Get();
				break;
			}
			case 56 /* "Y" */: {
				Get();
				break;
			}
			case 57 /* "O" */: {
				Get();
				break;
			}
			}
			lastToken = (wstring)t->val;
			//wcout << "Se agregara el siguiente operador: " << lastToken << endl;
			POperadores.push(lastToken);/* push de operador */
			
			EXPRESION();
		}
}

void Parser::MIENTRAS() {
		Expect(45 /* "mientras" */);
		PSaltos.push(contCuad);/*meter cont en PSaltos*/
		
		Expect(34 /* "(" */);
		EXPRESION();
		Expect(35 /* ")" */);
		Expect(46 /* "haz" */);
		if(PTipos.top() != 1)
		{
		PTipos.pop();
		wcout << t->line << L":ERROR!! Expresion en la linea " << t -> line << L" tiene que ser entera" << endl;
		error();
		}
		else
		{
		operando1 = POperandos.top();
		//wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
		POperandos.pop();
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo (L"falsoM", operando1, L"", L"")));
		//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		PSaltos.push(contCuad);
		contCuad++;
		}
		
		BLOQUE();
		finM = PSaltos.top();
		PSaltos.pop();
		tCuadruplos.find(finM)->second->setDestino(to_wstring((long double)contCuad));
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo (L"regresaM", L"", L"", to_wstring((long double)PSaltos.top()))));
		//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		PSaltos.pop();
		contCuad++;
		
}

void Parser::HAZ() {
		Expect(46 /* "haz" */);
		Expect(34 /* "(" */);
		VECES();
		varVeces = (wstring)t->val;
		if(tipoVeces != 1)
		{
		wcout << t->line << L":ERROR!! Tiene que haber un valor entero en el estatuto \"veces\"" << endl;
		error();
		}
		else
		{
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"inicioHAZ", varVeces, L"", L"")));
		//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		contCuad++;
		}
		
		Expect(35 /* ")" */);
		Expect(47 /* "veces" */);
		BLOQUE();
		tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(L"finHAZ", L"", L"", L"")));
		//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		contCuad++;
		
}

void Parser::VECES() {
		if (la->kind == _ENTERO) {
			Get();
			tipoVeces = 1;
		} else if (la->kind == _ID) {
			Get();
			nombreVariable = (wstring) t -> val;
			varKey = wstr_hash(nombreVariable);
			
			ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
			contRepeat = ptrTVars -> count(varKey);
			if(contRepeat != 0)
			{
			ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
			tipoVeces = ptrVariable->getTipo();								
			}
			else
			{
			ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
			ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
			ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
			contRepeat = ptrTVars -> count(varKey);
			if(contRepeat != 0)
			{
				ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
				tipoVeces = ptrVariable->getTipo();
			}
			else
			{
				wcout << t->line << L":ERROR!! La variable no se encuentra definida en el programa" << endl;
				error();
			}
			}							
							
			
		} else SynErr(70);
}

void Parser::LLPARAMETROS() {
		Expect(_ID);
		contParams++;
		if(contParams > ptrParametros->size())
		{
		cout << t->line << ":ERROR!! Hay mas argumentos que los parametros declarados en la funcion" << endl;
		error();
		}
		//wcout << "ContParams: " << contParams << endl;
		nombreVariable = (wstring)t->val;
		varKey = wstr_hash(nombreVariable);
		//wcout << "Variable parametro: " << nombreVariable << endl;
		//Necesario para buscar si esta declarada en Globales Nombre de variable 
		ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
		contRepeat = ptrTVars -> count(wstr_hash(nombreVariable));
		if(contRepeat != 0)
		{
		//Obtener el tipo de la variable global
		ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
		//wcout << "El nombre de la variable a la que se les checara el tipo es: " << ptrVariable->getNombre() << endl;
		tipoParametro = ptrVariable->getTipo();
		//wcout << "El tipo de " << nombreVariable << " es " << tipoParametro << endl;
		//Comparar con los tipos del mapa de Parametros
		//wcout << "Argumento " << (*itParams)->getNombre() << endl;
		tipoArgumento = (*itParams)->getTipo();
		//wcout << L"Tipo de Argumento es: " << tipoArgumento << endl;
		if(tipoParametro == 4 || tipoParametro == 5)
		{
			cout << t->line << ":ERROR!! Los arreglos no pueden ser usados como parametros" << endl;
			error();
		}
		
		if(tipoParametro == tipoArgumento)
		{
				ptrListaParametros->push_back(nombreVariable);
				itParams++;
		}
		else
		{
				cout << t->line << ":ERROR!! El argumento no corresponde con el tipo del parametro" << endl;
				error();
		}
		}
		else
		{
		//wcout << "Tipo de Argumento: " << tipoArgumento << endl;
		ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
		contRepeat = ptrTHW -> count(wstr_hash(nombreVariable));
		if(contRepeat != 0)
		{
			cout << t->line << ":ERROR!! Una variable de hardware no puede ser usada como argumento" << endl;
			error();
		}
		else
		{
			wcout << t->line << ":ERROR!! El parametro " << nombreVariable << " no esta declarado como variable" << endl;
			error();
		}
		
		}			
		
		if (la->kind == 26 /* "," */) {
			Get();
			LLPARAMETROS();
		}
}

void Parser::VARHW() {
		switch (la->kind) {
		case _HMOTOR: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 1)
			{
			wcout << t->line << "ERROR!! El atributo " << (wstring)t->val << " no es valido para este componente" << endl;
			error();
			}
			
			
			
			break;
		}
		case _HSENSOR: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 6)
			{
			wcout << t->line << L"ERROR!! El atributo " << (wstring)t->val << L" no es valido para este componente" << endl;
			error();
			}
			
			break;
		}
		case _HLED: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 4)
			{
			wcout << t->line << L"ERROR!! El atributo " << (wstring)t->val << L" no es valido para este componente" << endl;
			error();
			}
			
			
			break;
		}
		case _HPANTALLA: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 3)
			{
			wcout << t->line << L"ERROR!! El atributo " << (wstring)t->val << L" no es valido para este componente" << endl;
			error();
			}
			
			
			break;
		}
		case _HSERVO: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 2)
			{
			wcout << t->line << L"ERROR!! El atributo " << (wstring)t->val << L" no es valido para este componente" << endl;
			error();
			}
			
			break;
		}
		case _HBOTON: {
			Get();
			ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
			ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
			tipoHW = ptrComponente->getTipoHW();/*obtener el tipo del atributo de la variable de HW*/
			if(tipoHW != 5)
			{
			wcout << t->line << L"ERROR!! El atributo " << (wstring)t->val << L" no es valido para este componente" << endl;
			error();
			}
			
			
			break;
		}
		default: SynErr(71); break;
		}
}

void Parser::ASIGNACION() {
		Expect(49 /* "<-" */);
		lastToken = (wstring)t->val;
		//wcout << "Se agregara el siguiente operador: " << lastToken << endl;
		POperadores.push(lastToken);// push(=) asignacion
		//wcout << "Se agrego " << lastToken << " exitosamente" << endl;
		
		
		if (StartOf(7)) {
			EXPRESION();
		} else if (la->kind == _FRASE) {
			Get();
			lastToken = (wstring)t->val;POperandos.push(lastToken); PTipos.push(3);
		} else SynErr(72);
		if(POperadores.size() != 0)
		{
		if(POperadores.top() == L"<-")
		{
			//cout << "Entramos al if de ASIGNACION" << endl;
			operador = POperadores.top();
			//wcout << "Operador = " << POperadores.top() << endl;
			POperadores.pop();
			//wcout << "Pop de Operadores Hecho!!" << endl;
			operandoT1 = PTipos.top();
			//wcout << "OperandoT1 = " << PTipos.top() << endl;
			PTipos.pop();
			//wcout << "Pop de Tipos Hecho!!" << endl;
			destinoT = PTipos.top();
			//wcout << "Operando Destino = " << PTipos.top() << endl;
			PTipos.pop();
			revTipo = buscaCubo(operador, destinoT, operandoT1);
			if(revTipo == 13)
			{
				wcout << t->line << L":ERROR!! Tipos no compatibles en la linea " << t ->line  << endl;
				error();
			}
			else
			{
			    operando1 = POperandos.top();
				//wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
				POperandos.pop();
				destino = POperandos.top();
				//wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
				POperandos.pop();
				operando2 = L"";
				tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(operador, operando1, operando2, destino)));
				//cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
				contCuad++;
			}
		}
		}
		
}

void Parser::EXP() {
		TERMINO();
		if(POperadores.size() != 0)
		{
		if(POperadores.top() == L"+" || POperadores.top() == L"-")
		{
		operador = POperadores.top();
		//wcout << "Se esta a punto de sacar el operador " << POperadores.top() << endl;
		POperadores.pop();
		operandoT2 = PTipos.top();
		PTipos.pop();
		//cout << "El tipo de Operando2 es: " << operandoT2 << endl;
		operandoT1 = PTipos.top();
		PTipos.pop();
		//cout << "El tipo de Operando1 es: " << operandoT1 << endl;
		revTipo = buscaCubo(operador, operandoT1, operandoT2);/*revisar que los tipos sean permitidos*/
		//cout << "El tipo resulantes de Opdo1 y Opd2 es: " << revTipo << endl;
		if (revTipo == 13)
		{
			wcout << t->line << L":ERROR!! Tipos no compatibles en la linea " << t->line  << endl;
			error();
		}
		else
		{
		   operando2 = POperandos.top();
		   //wcout << "Se esta a punto de sacar el operando2 EXP " << POperandos.top() << endl;
		   POperandos.pop();
		   operando1 = POperandos.top();
		   //wcout << "Se esta a punto de sacar el operando1 EXP " << POperandos.top() << endl;
		   POperandos.pop();
		   strTemp = L"_t" + to_wstring((long double)contT);
		   destino = strTemp;
		   tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(operador, operando1, operando2, destino)));
		   //cout << "Se creo un nuevo cuadruplo. Contador --> " << contCuad << endl;
		   //wcout << "Se esta a punto de meter el operando EXP" << destino << endl;
		   POperandos.push(destino);
		   PTipos.push(revTipo);
		   contCuad++;
		   contT++;
		}
		}
		}
		
		if (la->kind == 37 /* "-" */ || la->kind == 58 /* "+" */) {
			if (la->kind == 58 /* "+" */) {
				Get();
			} else {
				Get();
			}
			lastToken = (wstring)t->val;
			//wcout << "Se agregara el siguiente operador: " << lastToken << endl;
			POperadores.push(lastToken);/*meter operador a POperadores*/
			
			EXP();
		}
}

void Parser::TERMINO() {
		FACTOR();
		if(POperadores.size() != 0)
		{
		if(POperadores.top() == L"*" || POperadores.top() == L"/")
		 {
		  operador = POperadores.top();
		  POperadores.pop();
		  operandoT2 = PTipos.top();
		  PTipos.pop();
		  operandoT1 = PTipos.top();
		  PTipos.pop();
		  revTipo = buscaCubo(operador, operandoT1, operandoT2); /*revisar que los tipos sean permitidos*/
		  if (revTipo == 13)
		  {
			wcout << t->line << L":ERROR!! Tipos no compatibles en la linea " << t->line << endl;
			error();
		  }
		  else
		  {
		   operando2 = POperandos.top();
		   //wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
		   POperandos.pop();
		   operando1 = POperandos.top();
		   //wcout << "Se esta a punto de sacar el operando " << POperandos.top() << endl;
		   POperandos.pop();
		   strTemp = L"_t" + to_wstring((long double)contT);
		   destino = strTemp;
		   tCuadruplos.insert(pair<int,Cuadruplo*>(contCuad,new Cuadruplo(operador, operando1, operando2, destino)));//Genera cuadruplo
		   //wcout << "Se esta a punto de meter el operando TERMINO " << destino << endl;
		   POperandos.push(destino);
		   PTipos.push(revTipo);
		   contCuad++;
		   contT++;
		   
		}
		 }
		}
		
		if (la->kind == 59 /* "*" */ || la->kind == 60 /* "/" */) {
			if (la->kind == 59 /* "*" */) {
				Get();
			} else {
				Get();
			}
			lastToken = (wstring)t->val;
			//wcout << "Se agregara el siguiente operador: " << lastToken << endl;
			POperadores.push(lastToken);/*meter operador a POperadores*/			  
			
			TERMINO();
		}
}

void Parser::FACTOR() {
		if (la->kind == 34 /* "(" */) {
			Get();
			POperadores.push(L"(");/*6 meter fondo falso*/
			//wcout << "Se agrego fondo falso" << endl;
			
			EXPRESION();
			Expect(35 /* ")" */);
			POperadores.pop();/*7 sacar fondo falso*/
			//wcout << "Se saco fondo falso" << endl;
			
		} else if (StartOf(8)) {
			if (la->kind == _ENTERO || la->kind == _DECIMAL || la->kind == 37 /* "-" */) {
				if (la->kind == 37 /* "-" */) {
					Get();
					signoEntero = (wstring)t->val;
				}
				CTE();
				lastToken = (wstring)t->val;
				tipoID = tipoConstante;
				idt = signoEntero + lastToken;
				signoEntero = L"";
				
			} else {
				Get();
				idt = (wstring)t->val;/*var aux para id*/
				
				// Corroborar el tipo de ID
				nombreVariable = (wstring) t -> val;
				varKey = wstr_hash(nombreVariable);
				
				ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
				contRepeat = ptrTVars -> count(varKey);
				if(contRepeat != 0)
				{
				ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
				tipoID = ptrVariable->getTipo();	
				//wcout << "El tipo de la variable global es: " << tipoID << endl;
				}
				else
				{
				if(varScope == L"Local")
				{
					ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
					ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
					ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
					contRepeat = ptrTVars -> count(varKey);
					if(contRepeat != 0)
					{
						ptrVariable = (Variable*) ptrTVars->find(varKey)->second;
						tipoID = ptrVariable->getTipo();
												
					}
					else
					{
						ptrParametros = (Parametros*) ptrTDataF->find(L"fParams")->second;
						for(Parametros::iterator itP=ptrParametros->begin();itP != ptrParametros->end();itP++)
						{
							if((*itP)->getNombre() == nombreVariable)
							{
								encontreParam = true;
								tipoID = (*itP)->getTipo();
							}
						}
						
						if(!encontreParam)
						{
							ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
							contRepeat = ptrTHW->count(varKey);/*obtener el tipo del atributo de la variable de HW*/
							if(contRepeat != 0)
							{
								esTipoHW = true;
							}
							else
							{
								wcout << t->line << ":ERROR!! La variable " << nombreVariable << " no esta declarada" << endl;
							}
						}
						
						encontreParam = false;
					
					}
				}
				else
				{
					ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
						contRepeat = ptrTHW->count(varKey);/*obtener el tipo del atributo de la variable de HW*/
						if(contRepeat != 0)
						{
							esTipoHW = true;
						}
						else
						{
							wcout << t->line << ":ERROR!! La variable " << nombreVariable << " no esta declarada" << endl;
						}	
				
				}
										
				}						
				
				
				if (StartOf(4)) {
					if (la->kind == 48 /* "posicion" */) {
						Get();
						if (la->kind == _ENTERO) {
							Get();
							vinoEnt = true;
						} else if (la->kind == _ID) {
							Get();
						} else SynErr(73);
						lastToken = (wstring)t->val;	
						
						if(tipoID == 4 || tipoID == 5)
						{
						//--------------------------
						if(!vinoEnt)
						{
							ptrTVars = (tablaVars*) tablaScope.find(wstr_hash(L"Global")) -> second;
							contRepeat = ptrTVars -> count(wstr_hash(lastToken));
							if(contRepeat != 0)
							{
								ptrVariable = (Variable*) ptrTVars->find(wstr_hash(lastToken))->second;
								arregloTipoID = ptrVariable->getTipo();
							}
							else
							{	
								if(varScope == L"Local")
								{
									ptrTProcs = (tablaProcs*) tablaScope.find(wstr_hash(L"Local")) -> second;
									ptrTDataF = (tablaDataFunc*) ptrTProcs -> find(procKey) -> second;
									ptrTVars = (tablaVars*) ptrTDataF -> find (L"fVars") -> second;
									contRepeat = ptrTVars -> count(wstr_hash(lastToken));
									if(contRepeat != 0)
									{
										ptrVariable = (Variable*) ptrTVars->find(wstr_hash(lastToken))->second;
										arregloTipoID = ptrVariable->getTipo();
									}
									else
									{
										wcout << t->line << L":ERROR!! La variable para indexar el arreglo " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
										error();
									}
								}
								else
								{
									wcout << t->line << L":ERROR!! La variable para indexar el arreglo " << accionHWoPos << " no se encuentra definida o es de tipo Hardware" << endl;
									error();
								}
							}
								
								
							if(arregloTipoID != 1)
							{	
								cout << t->line << L":ERROR!! La variable utilizada para indexar en el arreglo no es de tipo entero!" << endl;
								error();
							}
							else
							{
								idt = idt + L"[" + lastToken + L"-1]";
								if(tipoID==4)
								tipoID = 1;
								else
								tipoID = 2;
							}
						}
						else
						{
							arrCasillas = ptrVariable->getNumCasillas();
							if(stoi(lastToken) <= arrCasillas && stoi(lastToken) > 0)
							{	
								idt = idt + L"[" + lastToken + L"-1]";
								if(tipoID==4)
									tipoID = 1;
								else
									tipoID = 2;
							}
							else
							{
								cout << t->line <<":ERROR!!! Fuera del rango del arreglo" << endl;
								error();
							}
						}
						}
						else
						{
						wcout << t->line <<L":ERROR!! La variable " << nombreVariable <<L" no es un arreglo " << endl;
						error();
						}
						
						vinoEnt = false;
						
					} else {
						if(!esTipoHW)
						{
						wcout << t->line << L":ERROR!! La variable " << nombreVariable << L" no es de tipo hardware" << endl;
						error();
						}
						
						VARHW();
						lastToken = (wstring)t->val;
						
						ptrTHW = (tablaHardware*)tablaScope.find(wstr_hash(L"Hardware")) -> second;/*apuntador a la tabla de variables de Hardware*/
						ptrComponente = (Componente*) ptrTHW->find(wstr_hash(idt))->second;/*encuentra la variable de hardware*/
						tipoID = ptrComponente->getTipo(lastToken);/*obtener el tipo del atributo de la variable de HW*/
						idt = idt + lastToken;/*cambiar tipo de token para que incluya HW*/
						
					}
				}
			}
			POperandos.push(idt); /*Push pila deoperandos*/
			//wcout << "El top de POperandos en FACTOR es: " << POperandos.top();
			PTipos.push(tipoID);/*Push pila de tipos*/
			//wcout << "El top de PTipos en FACTOR es: " << PTipos.top();
			esTipoHW = false;
			
			
		} else SynErr(74);
}

void Parser::CTE() {
		if (la->kind == _ENTERO) {
			Get();
			tipoConstante = 1;
		} else if (la->kind == _DECIMAL) {
			Get();
			tipoConstante = 2;
		} else SynErr(75);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	EASYHW();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 61;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[9][63] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,T, T,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,T, T,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,T, T,T,x,x, x,x,x},
		{x,T,T,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,x, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ID expected"); break;
			case 2: s = coco_string_create(L"ENTERO expected"); break;
			case 3: s = coco_string_create(L"DECIMAL expected"); break;
			case 4: s = coco_string_create(L"FRASE expected"); break;
			case 5: s = coco_string_create(L"LETRA expected"); break;
			case 6: s = coco_string_create(L"ENDL expected"); break;
			case 7: s = coco_string_create(L"HMOTOR expected"); break;
			case 8: s = coco_string_create(L"HSENSOR expected"); break;
			case 9: s = coco_string_create(L"HLED expected"); break;
			case 10: s = coco_string_create(L"HPANTALLA expected"); break;
			case 11: s = coco_string_create(L"HSERVO expected"); break;
			case 12: s = coco_string_create(L"HBOTON expected"); break;
			case 13: s = coco_string_create(L"PINANALOG expected"); break;
			case 14: s = coco_string_create(L"\"CONFIG\" expected"); break;
			case 15: s = coco_string_create(L"\"{\" expected"); break;
			case 16: s = coco_string_create(L"\"}\" expected"); break;
			case 17: s = coco_string_create(L"\"VARG\" expected"); break;
			case 18: s = coco_string_create(L"\"FUNCIONES\" expected"); break;
			case 19: s = coco_string_create(L"\"PROGRAMA\" expected"); break;
			case 20: s = coco_string_create(L"\"motor\" expected"); break;
			case 21: s = coco_string_create(L"\"servo\" expected"); break;
			case 22: s = coco_string_create(L"\"pantalla\" expected"); break;
			case 23: s = coco_string_create(L"\"led\" expected"); break;
			case 24: s = coco_string_create(L"\"boton\" expected"); break;
			case 25: s = coco_string_create(L"\"sensor\" expected"); break;
			case 26: s = coco_string_create(L"\",\" expected"); break;
			case 27: s = coco_string_create(L"\"pin\" expected"); break;
			case 28: s = coco_string_create(L"\"espacios\" expected"); break;
			case 29: s = coco_string_create(L"\"entero\" expected"); break;
			case 30: s = coco_string_create(L"\"decimal\" expected"); break;
			case 31: s = coco_string_create(L"\"frase\" expected"); break;
			case 32: s = coco_string_create(L"\"enteroA\" expected"); break;
			case 33: s = coco_string_create(L"\"decimalA\" expected"); break;
			case 34: s = coco_string_create(L"\"(\" expected"); break;
			case 35: s = coco_string_create(L"\")\" expected"); break;
			case 36: s = coco_string_create(L"\"VARF\" expected"); break;
			case 37: s = coco_string_create(L"\"-\" expected"); break;
			case 38: s = coco_string_create(L"\"condicion\" expected"); break;
			case 39: s = coco_string_create(L"\"ciclo\" expected"); break;
			case 40: s = coco_string_create(L"\"llamar\" expected"); break;
			case 41: s = coco_string_create(L"\"asignar\" expected"); break;
			case 42: s = coco_string_create(L"\"esperar\" expected"); break;
			case 43: s = coco_string_create(L"\"si\" expected"); break;
			case 44: s = coco_string_create(L"\"sino\" expected"); break;
			case 45: s = coco_string_create(L"\"mientras\" expected"); break;
			case 46: s = coco_string_create(L"\"haz\" expected"); break;
			case 47: s = coco_string_create(L"\"veces\" expected"); break;
			case 48: s = coco_string_create(L"\"posicion\" expected"); break;
			case 49: s = coco_string_create(L"\"<-\" expected"); break;
			case 50: s = coco_string_create(L"\">\" expected"); break;
			case 51: s = coco_string_create(L"\"<\" expected"); break;
			case 52: s = coco_string_create(L"\"<=\" expected"); break;
			case 53: s = coco_string_create(L"\">=\" expected"); break;
			case 54: s = coco_string_create(L"\"!=\" expected"); break;
			case 55: s = coco_string_create(L"\"=\" expected"); break;
			case 56: s = coco_string_create(L"\"Y\" expected"); break;
			case 57: s = coco_string_create(L"\"O\" expected"); break;
			case 58: s = coco_string_create(L"\"+\" expected"); break;
			case 59: s = coco_string_create(L"\"*\" expected"); break;
			case 60: s = coco_string_create(L"\"/\" expected"); break;
			case 61: s = coco_string_create(L"??? expected"); break;
			case 62: s = coco_string_create(L"invalid COMPONENTE"); break;
			case 63: s = coco_string_create(L"invalid NUMPIN"); break;
			case 64: s = coco_string_create(L"invalid DECLARACIONES"); break;
			case 65: s = coco_string_create(L"invalid TIPO"); break;
			case 66: s = coco_string_create(L"invalid TIPOA"); break;
			case 67: s = coco_string_create(L"invalid ESTATUTOS"); break;
			case 68: s = coco_string_create(L"invalid CICLOS"); break;
			case 69: s = coco_string_create(L"invalid ASIGN"); break;
			case 70: s = coco_string_create(L"invalid VECES"); break;
			case 71: s = coco_string_create(L"invalid VARHW"); break;
			case 72: s = coco_string_create(L"invalid ASIGNACION"); break;
			case 73: s = coco_string_create(L"invalid FACTOR"); break;
			case 74: s = coco_string_create(L"invalid FACTOR"); break;
			case 75: s = coco_string_create(L"invalid CTE"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


