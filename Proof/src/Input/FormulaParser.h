#ifndef FormulaParser_h
#define FormulaParser_h

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"

namespace FormulaParser
{
	IFormula * ParseFormula(string str);
	IFormula * ParseTemp(string str);
	IFormula * ReadFormula(string::iterator& it, string::iterator& end, bool temp);
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end, bool temp);

	IFormulaSet * ParseFormulaSet(string str, FSetType type);
	AxiomContainer * ParseAxioms(string str);

	template<typename T>
	bool ReadFormulaSet(string::iterator& it, string::iterator& end, bool temp, T * formulas);
}

#endif