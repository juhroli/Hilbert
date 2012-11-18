#include "../../HilbertIncludes.h"
#include "FalseFormula.h"

FalseFormula::FalseFormula()
	: AtomicFormula((char*)FALSE)
{
	m_value = false;
}

FalseFormula::FalseFormula(FalseFormula& formula)
{
	m_symbol = (char*)FALSE;
	m_value = false;
	m_hash = formula.HashCode();
}

bool FalseFormula::IsTemp()
{
	return false;
}

bool FalseFormula::Eval()
{
	return false;
}

/*
*	False can't be cloned.
*/
IFormula * FalseFormula::Clone()
{
	return this;
}