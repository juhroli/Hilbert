#include "AtomicFormula.h"

AtomicFormula::AtomicFormula()
	: m_symbol(__nullptr)
	, m_value(true)
	, m_hash(0)
{
}

AtomicFormula::AtomicFormula(char * symbol)
	: m_value(true)
{
	m_symbol = new char[strlen(symbol) + 1];
	strcpy_s(m_symbol, strlen(symbol) + 1, symbol);
	//Generate hash code
	string fs = this->ToString();
	locale loc;
	const collate<char>& coll = use_facet<collate<char>>(loc);
	m_hash = coll.hash(fs.data(), fs.end()._Ptr);
}

AtomicFormula::AtomicFormula(AtomicFormula& formula)
{
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_hash = formula.HashCode();
}

AtomicFormula::~AtomicFormula()
{
	if(m_symbol != __nullptr)
	{
		delete[] m_symbol;
		m_symbol = __nullptr;
	}
}

bool AtomicFormula::IsAtomic()
{
	return true;
}

bool AtomicFormula::IsTemp()
{
	return false;
}

bool AtomicFormula::Eval()
{
	return m_value;
}

bool AtomicFormula::Equals(IFormula * formula)
{
	if(!formula && !formula->IsAtomic())
		return false;
	
	return m_hash == formula->HashCode();
}

string AtomicFormula::ToString()
{
	return m_symbol;
}

IFormula * AtomicFormula::Clone()
{
	return new AtomicFormula(*this);
}

bool AtomicFormula::IsNull()
{
	return m_symbol == __nullptr;
}

IFormula * AtomicFormula::Replace(IFormula& t, IFormula& x)
{
	return this;
}

void AtomicFormula::SetValue(bool value)
{
	m_value = value;
}

void AtomicFormula::NegValue()
{
	m_value = !m_value;
}

char * AtomicFormula::GetSymbol()
{
	return m_symbol;
}

unsigned AtomicFormula::Length()
{
	return 1;
}

long AtomicFormula::HashCode()
{
	return m_hash;
}