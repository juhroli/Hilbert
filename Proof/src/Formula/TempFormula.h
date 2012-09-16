#pragma once

#include "AtomicFormula.h"

class TempFormula : public AtomicFormula
{
public:
	TempFormula();
	TempFormula(char * symbol);
	TempFormula(TempFormula& formula);
	~TempFormula() {}

	bool IsTemp();
	IFormula * Clone();
};