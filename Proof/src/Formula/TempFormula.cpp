#include "TempFormula.h"
#include <hash_map>

using std::hash;

TempFormula::TempFormula()
	: AtomicFormula()
{
}

TempFormula::TempFormula(char * symbol)
	: AtomicFormula(symbol)
{
	stringstream stream;
	stream<<'_'<<m_symbol<<'\0';
	int len = stream.str().length();
	m_symbol = new char[len];
	stream.str().copy(m_symbol, len);
}

TempFormula::TempFormula(TempFormula& formula)
{
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_id = formula.GetId();
}

bool TempFormula::IsTemp()
{
	return true;
}

bool TempFormula::Equals(IFormula * formula)
{
	if(!formula->IsAtomic() && !formula->IsTemp())
		return false;
	
	return m_id == static_cast<AtomicFormula*>(formula)->GetId();
}

string TempFormula::ToString()
{

	return &(m_symbol[1]);
}

IFormula * TempFormula::Clone()
{
	return new TempFormula(*this);
}