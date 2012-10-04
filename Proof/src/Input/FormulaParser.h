#pragma once

#include "../Formula/IFormula.h"

namespace FormulaParser
{
	IFormula * ParseFormula(string str);
	IFormula * ReadFormula(string::iterator& it, string::iterator& end);
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end);
}