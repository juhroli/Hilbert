#include <iostream>
#include <string>

#include "FormulaGenerator.h"
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout << "Error: Argument needed for the maximum number of formulas." << endl;
		return 0;
	}

	FormulaGenerator gen(atoi(argv[1]));

	cout << gen.Generate() << endl;

	return 0;
}