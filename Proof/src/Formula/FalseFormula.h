#pragma once

#include "AtomicFormula.h"

class FalseFormula : public AtomicFormula
{
public:
	FalseFormula()
		: AtomicFormula(0, "~")
	{
	}
	bool IsAtomic();
	bool Eval();
	string ToString();
protected:
	
};