#pragma once

#include "IFormula.h"

class AtomicFormula : public IFormula
{
public:
	AtomicFormula();
	AtomicFormula(char * symbol);
	~AtomicFormula();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	char * ToString();
	bool Equals(IFormula * formula);
	void SetValue(bool value);
	void NegValue(); //Negates current value
	virtual char * GetSymbol();

protected:
	char * m_symbol;
	bool m_value;
};