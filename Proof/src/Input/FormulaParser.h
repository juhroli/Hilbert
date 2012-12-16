#ifndef FormulaParser_h
#define FormulaParser_h

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"

namespace FormulaParser
{
	IFormula * ParseFormula(string str);
	IFormula * ParseTemp(string str);
	IFormula * ReadFormula(string::iterator& it, string::iterator& end, bool temp);
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end, bool temp);

	IFormulaSet * ParseFormulaSet(string str, FSetType type);
	bool ReadFormulaSet(string::iterator& it, string::iterator& end, bool temp, IFormulaSet * fset);
}

#endif