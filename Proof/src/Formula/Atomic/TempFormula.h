#ifndef TempFormula_h
#define TempFormula_h

#include "AtomicFormula.h"

class TempFormula : public AtomicFormula
{
public:
	TempFormula();
	TempFormula(char * symbol);
	TempFormula(TempFormula& formula);
	~TempFormula() {}

	bool IsTemp();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	IFormula * Replace(IFormula * x, IFormula * t);
};

#endif