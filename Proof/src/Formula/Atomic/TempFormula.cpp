#include "../../HilbertIncludes.h"
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
	int len = stream.str().length();
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
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_hash = formula.HashCode();
}

bool TempFormula::IsTemp()
{
	return true;
}

bool TempFormula::Equals(IFormula * formula)
{
	if(!formula || !formula->IsAtomic() || !formula->IsTemp())
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
IFormula * TempFormula::Replace(IFormula * t, IFormula * x)
{
	if(t == nullptr || x == nullptr)
		return this;

	if(this->Equals( t ))
		return x->IsAtomic() ? x : x->Clone();

	return this;
}