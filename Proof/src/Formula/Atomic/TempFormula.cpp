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
	strcpy_s(m_symbol, len, stream.str().c_str());

	//Generate hash code
	string fs = this->GetSymbol();
	locale loc;
	const collate<char>& coll = use_facet<collate<char>>(loc);
	m_hash = coll.hash(fs.data(), fs.end()._Ptr);
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
	if(!formula && !formula->IsAtomic() && !formula->IsTemp())
		return false;
	
	return m_hash == formula->HashCode();
}

string TempFormula::ToString()
{

	return &(m_symbol[1]);
}

IFormula * TempFormula::Clone()
{
	return new TempFormula(*this);
}

IFormula * TempFormula::Replace(IFormula& t, IFormula& x)
{
	if(this->Equals( &t ))
		return &x;
	return this;
}