#include <iostream>
#include <string>
#include <cmath>

#include "FormulaGenerator.h"
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	try
	{
		if(argc < 2)
			throw "Error 1: No arguments. First argument is the maximum number of atomics in a formula, second is the random seed.";
		
		unsigned n = atoi(argv[1]);

		bool lenneq = strlen(argv[1]) > unsigned(log10(n)) + 1;

		if(lenneq || (!lenneq && *argv[1] != '0' && n == 0) || atoi(argv[1]) < 0)
			throw "Error 2: Arg1 is not a valid argument.";

		FormulaGenerator gen;
		
		if(argc > 2)
		{
			unsigned seed;

			stringstream(argv[2]) >> seed;
			
			if(strlen(argv[2]) > unsigned(log10(seed)) + 1 || seed < 0)
				throw "Error 3: Arg2 is not a valid argument.";

			gen = FormulaGenerator(n, seed);

		}
		else
			gen = FormulaGenerator(n);

		cout << gen.Generate() << endl;
	}
	catch(char * error)
	{
		cout << error << endl;
	}
	catch(char const * error)
	{
		cout << error << endl;
	}

	return 0;
}