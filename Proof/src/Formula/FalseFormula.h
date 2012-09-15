#pragma once

#include "AtomicFormula.h"

class FalseFormula : public AtomicFormula
{
public:
	FalseFormula();
	FalseFormula(FalseFormula& formula);
	bool IsTemp();
	bool Eval();
	IFormula * Clone();
};