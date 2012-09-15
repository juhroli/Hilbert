#include "FalseFormula.h"

FalseFormula::FalseFormula()
	: AtomicFormula((char*)FALSE, 0)
{
	m_value = false;
}

FalseFormula::FalseFormula(FalseFormula& formula)
{
	m_symbol = (char*)FALSE;
	m_value = false;
	m_hash = 0;
}

bool FalseFormula::IsTemp()
{
	return false;
}

bool FalseFormula::Eval()
{
	return false;
}

IFormula * FalseFormula::Clone()
{
	return new FalseFormula(*this);
}