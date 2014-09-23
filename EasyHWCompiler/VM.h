for(int i = 1; i <= tCuadruplos.size(); i++)
{
	operVM = tCuadruplos.find(i)->second->getOper();
	if(L"<-" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		find1 = opdoVM1.find(L".");
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		find2 = destinationVM.find(L".");
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			destinationVM = pinb + campob;
			archLoop << destinationVM << L" = " << opdoVM1 << L";" << endl;
			escribeHW(pinb, campob, destinationVM);
		}
		else
		{
			archLoop << destinationVM << L" = " << opdoVM1 << L";" << endl;
		}
	} 
	else if(L"<" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		opdoVM2 = tCuadruplos.find(i)->second->getOp2();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		find1 = opdoVM1.find(L".");
		find2 = opdoVM2.find(L".");
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
		}
		tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" + " + opdoVM2 + L")";
		archDef << tempVM << endl;
	} 
	else if(L"-" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		opdoVM2 = tCuadruplos.find(i)->second->getOp2();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		find1 = opdoVM1.find(L".")
		find2 = opdoVM2.find(L".");
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
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
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		if (find2)
		{
			pinb = opdoVM2.substr(0,find2);
			campob = opdoVM2.substr(find2);
			opdoVM2 = pinb + campob;
		}
		tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + L" / " + opdoVM2 + L")";
		archDef << tempVM << endl;
	} 
	/*else if(L"NO" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		tempVM = L"#define " + destinationVM + L" (" + opdoVM1 + operVM + opdoVM2 + L")";
	} */
	else if(L"inicioM" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		find1 = opdoVM1.find(L".");
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		tempVM = L"while(" + opdoVM1 + L")";
		archLoop << tempVM << endl << L"{" << endl;
	} 
	else if(L"finM" == operVM)
	{
		archLoop << L"}" << endl;
	} 
	else if(L"inicioHAZ" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		tempVM = L"for(int i = 0; i < " + opdoVM1 + L"; i++)";
		archLoop << tempVM << endl << L"{"<< endl;
	} 
	else if(L"finHAZ" == operVM)
	{
		archLoop << L"}" << endl;
	} 
	else if(L"inicioIf" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		find1 = opdoVM1.find(L".");
		if(find1)
		{
			pina = opdoVM1.substr(0,find1-1);
			campoa = opdoVM1.substr(find1+1);
			opdoVM1 = pina + campoa;
		}
		archIno << L"if(" + opdoVM1 + L")" << endl;
		archIno << L"{" << endl;
	}
	else if(L"finIf" == operVM)
	{
		archIno << L"}" << endl;
	}
	else if(L"inicioElse" == operVM)
	{
		archIno << L"else" << endl << L"{" << endl;
	}
	else if(L"finElse" == operVM)
	{
		archIno << L"}" << endl;
	}
	else if(L"llamaFunc" == operVM)
	{
		opdoVM1 = tCuadruplos.find(i)->second->getOp1();
		destinationVM = tCuadruplos.find(i)->second->getDestino();
		archLoop << destinationVM << L"(";
		ptrParam = tLlParametros.find(stoi(opdoVM1))->second;
		for(i = 0; i < ptrParam->size() - 1; i++)
		{
			ptrParam = tLlParametros.find(stoi(opdoVM1))->second;
			nombrePaux = ptrParam->front();
			ptrParam->pop_front();
			archLoop << nombrePaux << L",";
		}
		ptrParam++;
		nombrePaux = ptrParam->front();
		archLoop << nombrePaux << L")" << endl;
	}
}
archLoop << L"}";

//funcion para hardware
escribeHW(wstring auxpin, wstring auxcampo, wstring destino)
{
	if(auxcampo == L"velocidad")
	{
		archLoop << L"var_escritura = limites(destino)" << endl;
		archLoop << L"analogWrite(" << auxpin << L",var_escritura" << ");" << endl;	
	}
	if(auxcampo == L"direccion")
	{
		archLoop << L"var_escritura = limites(destino)" << endl;
		archLoop << L"if( var_escritura = 0)" << endl << "{" << endl;
		//---Accion---------
		archLoop << L"digitalWrite(" << auxpin + L"_fwd" << L",LOW);" << endl;
		archLoop << L"digitalWrite(" << auxpin + L"_bwd" << L",HIGH);" << endl;
		archLoop << "}" << endl;
		//------------------
		archLoop << "else" << endl << "{" << endl; //Cuando es mayor a cero
		//----Accion--------
		archLoop << L"digitalWrite(" << auxpin + L"_fwd" << L",HIGH);" << endl;
		archLoop << L"digitalWrite(" << auxpin + L"_bwd" << L",LOW);" << endl;
		//------------------		
		archLoop << "}" << endl;
	}
	if(auxcampo == L"estado")
	{
		cout << "No puedes escribir en un pin de entrada" << endl;
		error();
	}
	if(auxcampo == L"encendido")
	{
		archLoop << L"var_escritura = limites(destino)" << endl;
		archLoop << L"if(var_escritura= 0)" << endl; //Cuando es igual a cero
		//----Accion-----
		archLoop << L"digitalWrite(" << auxpin << L",LOW);" << endl;
		//---------------
		archLoop << L"else" << endl; //Cuando es mayor a cero
		//----Accion------
		archLoop << L"digitalWrite(" << auxpin << L",HIGH);" << endl;
		//----------------
	}
	if(auxcampo == L"imprime")
	{
		//rutina para impresion en LCD
	}
	if(auxcampo == L"angulo")
	{
		archLoop << L"var_escritura = limites(destino)" << endl;
		archLoop << L"if(var_escritura > 180)" << endl;
		archLoop << L"var_escritura = 180;" << endl;
		archLoop << auxpin << L"." << L"write(var_escritura);" << endl;
	}
	if(auxcampo == L"lectura" )
	{
		cout << "No puedes escribir en un pin de entrada" << endl;
		error();
	}
}
//Funcion para limites
//falta declarar la variable var_escritura
archLoop << L"limites(int num)" << endl; 
archLoop << L"{" << endl;
archLoop << L"if(num<0)"<< endl << L"num = 0;" << endl; // para menor que 0
archLoop << L"else if(num>255)"<< endl << L"num = 255;" << endl; // para mayor que 255
archLoop << L"return(num);" << endl;
archLoop << L"}" << endl;

