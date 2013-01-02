#include <iostream>
#include <string>

#include "FormulaGenerator.h"
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	FormulaGenerator gen(atoi(argv[1]));

	cout << gen.Generate() << endl;

	return 0;
}