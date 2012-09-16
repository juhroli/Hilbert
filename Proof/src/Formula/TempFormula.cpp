#include "TempFormula.h"

TempFormula::TempFormula()
	: AtomicFormula()
{
}

TempFormula::TempFormula(char * symbol)
	: AtomicFormula(symbol)
{
	m_hash += 1;
}

TempFormula::TempFormula(TempFormula& formula)
{
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_hash = formula.GetHash();
}

bool TempFormula::IsTemp()
{
	return true;
}

IFormula * TempFormula::Clone()
{
	return new TempFormula(*this);
}