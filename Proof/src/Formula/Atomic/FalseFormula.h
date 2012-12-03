#ifndef FalseFormula_h
#define FalseFormula_h

#include "AtomicFormula.h"

class FalseFormula : public AtomicFormula
{
public:
	FalseFormula();
	FalseFormula(FalseFormula& formula);
	~FalseFormula() {}
	bool Eval();
	IFormula * Clone();
};

#endif