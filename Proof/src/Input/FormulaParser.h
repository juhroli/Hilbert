#ifndef FormulaParser_h
#define FormulaParser_h

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"

namespace FormulaParser
{
	IFormula * ParseFormula(string str);
	IFormula * ParseTemp(string str);
	IFormula * ReadFormula(string::iterator& it, string::iterator& end, bool temp);
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end, bool temp);
}

#endif