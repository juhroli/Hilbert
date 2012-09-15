#pragma once

#include "IFormula.h"

class AtomicFormula : public IFormula
{
public:
	AtomicFormula();
	AtomicFormula(char * symbol);
	AtomicFormula(char * symbol, unsigned hash);
	AtomicFormula(AtomicFormula& formula);
	virtual ~AtomicFormula();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	bool IsNull();

	void SetValue(bool value);
	void NegValue(); //Negates current value
	void SetHash(unsigned hash);
	unsigned GetHash();
	virtual char * GetSymbol();

protected:
	char * m_symbol;
	unsigned m_hash;
	bool m_value;
};