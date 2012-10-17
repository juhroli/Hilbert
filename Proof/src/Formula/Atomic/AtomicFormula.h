#pragma once

#include "../IFormula.h"

class AtomicFormula : public IFormula
{
public:
	AtomicFormula();
	AtomicFormula(char * symbol);
	AtomicFormula(AtomicFormula& formula);
	~AtomicFormula();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	bool IsNull();
	IFormula * Replace(IFormula& t, IFormula& x);
	unsigned Length();
	long HashCode();

	void SetValue(bool value);
	void NegValue(); //Negates current value
	char * GetSymbol();

protected:
	char * m_symbol;
	bool m_value;
	long m_hash;
};