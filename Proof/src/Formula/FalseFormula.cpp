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
	m_id = formula.GetId();
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