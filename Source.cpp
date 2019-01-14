/*
Dallas McGroarty
CPSC 323-85
Visual Studio 2015
*/
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Parser.h"

using namespace std;


int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cout << "Missing File Name \n";
		return 1;
	}
	ifstream fin(argv[1]);
	if (!fin)
	{
		cout << "File Not Found \n";
		return 1;
	}
	parser(fin);
	fin.close();

	return 0;
}