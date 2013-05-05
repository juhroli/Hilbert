#include "../../HilbertDefines.h"
#include "TempFormula.h"

TempFormula::TempFormula()
	: AtomicFormula()
{
}

TempFormula::TempFormula(char * symbol)
	: AtomicFormula()
{
	m_symbol = symbol;
	stringstream stream;
	stream<<'_'<<m_symbol<<'\0';
	size_t len = stream.str().length() + 1;
	m_symbol = new char[len];

#ifdef _MSC_VER
	strcpy_s(m_symbol, len, stream.str().c_str());
#else
	strcpy(m_symbol, stream.str().c_str());
#endif	

	//Generate hash code
	m_hash = GenerateHashCode(m_symbol);
}

TempFormula::TempFormula(TempFormula& formula)
{
	size_t len = strlen(formula.m_symbol) + 1;
	m_symbol = new char[len];

#ifdef _MSC_VER
	strcpy_s(m_symbol, len, formula.m_symbol);
#else
	strcpy(m_symbol, formula.m_symbol);
#endif

	m_value = formula.Eval();
	m_hash = formula.HashCode();
}

bool TempFormula::IsTemp()
{
	return true;
}

bool TempFormula::Equals(IFormula * formula)
{
	if(formula == nullptr || !formula->IsAtomic() || !formula->IsTemp())
		return false;
	
	return m_hash == formula->HashCode();
}

string TempFormula::ToString()
{

	return &(m_symbol[1]);
}

IFormula * TempFormula::Clone()
{
	return this;
}

/*
*	If x is atomic then return x (there won't be another instance of it) else clone x (it must have been a compound formula)
*/
IFormula * TempFormula::Replace(IFormula * x, IFormula * t)
{
	if(t == nullptr || x == nullptr)
		return this;

	if(this->Equals( x ))
		return t->IsAtomic() ? t : t->Clone();

	return this;
}