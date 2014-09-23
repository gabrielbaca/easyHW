#include <iostream>
#include "Parser.h"
#include "Scanner.h"

using namespace std;
int main(int argc, char *argv[])
{
	FILE* fuente = fopen("Fuente.txt","r");
	Scanner *miScanner = new Scanner(fuente);
	Parser *miParser = new Parser(miScanner);
	miParser->Parse();
	delete miParser;
	delete miScanner;
	system("PAUSE");
	return 0;
}