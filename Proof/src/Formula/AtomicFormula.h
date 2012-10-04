#pragma once

#include "IFormula.h"

class AtomicFormula : public IFormula
{
public:
	AtomicFormula();
	AtomicFormula(char * symbol);
	AtomicFormula(AtomicFormula& formula);
	virtual ~AtomicFormula();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	bool IsNull();
	IFormula * Replace(IFormula& t, IFormula& x);

	void SetValue(bool value);
	void NegValue(); //Negates current value
	unsigned GetId();
	void SetId(unsigned id);
	char * GetSymbol();

protected:
	char * m_symbol;
	unsigned m_id;
	bool m_value;
};