#pragma once

#include "../Formula/IFormula.h"

namespace InputHandler
{
	enum FormulaType
	{
		F_ATOMIC,
		F_IMPLICATION,
		F_AXIOM,
		F_TEMP,
		F_FALSE
	};

	IFormula * StringToFormula(string str);
	IFormula * StringToFormula(FormulaType type, string str);

}