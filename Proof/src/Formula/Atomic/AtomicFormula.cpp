#include "../../HilbertDefines.h"
#include "AtomicFormula.h"
#include <malloc.h>

AtomicFormula::AtomicFormula()
	: m_symbol(nullptr)
	, m_value(true)
	, m_hash(0)
{
}

AtomicFormula::AtomicFormula(char * symbol)
	: m_value(true)
{
	m_symbol = new char[strlen(symbol) + 1];
#ifdef _MSC_VER
	strcpy_s(m_symbol, strlen(symbol) + 1, symbol);
#else
	strcpy(m_symbol, symbol);
#endif
	//Generate hash code
	m_hash = GenerateHashCode(m_symbol);
}

AtomicFormula::AtomicFormula(AtomicFormula& formula)
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

AtomicFormula::~AtomicFormula()
{
	if(m_symbol != nullptr)
	{
		delete[] m_symbol;
		m_symbol = nullptr;
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
	if(formula == nullptr || !formula->IsAtomic())
		return false;
	
	return m_hash == formula->HashCode();
}

string AtomicFormula::ToString()
{
	return m_symbol;
}

IFormula * AtomicFormula::Clone()
{
	return this;
}

IFormula * AtomicFormula::Replace(IFormula * x, IFormula * t)
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

bool AtomicFormula::IsWrapped()
{
	return false;
}