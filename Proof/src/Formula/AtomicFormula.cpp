#include "AtomicFormula.h"

AtomicFormula::AtomicFormula()
	: IFormula()
{
	m_symbol = NULL;
	m_value = false;
	m_hash = -1;
}

AtomicFormula::AtomicFormula(char * symbol)
	: IFormula()
	, m_symbol(symbol)
	, m_value(true)
{
	m_hash = 0;
	unsigned len = strlen(symbol);

	for(unsigned i = 0; i < len; i++)
	{
		m_hash = 31 * m_hash + symbol[ i ];
	}

	m_hash = 31 * m_hash;
}

AtomicFormula::AtomicFormula(AtomicFormula& formula)
{
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_hash = formula.GetHash();
}

AtomicFormula::~AtomicFormula()
{
	m_symbol = NULL;
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
	if(!formula->IsAtomic())
		return false;
	
	return m_hash == static_cast<AtomicFormula*>(formula)->GetHash();
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
	return m_symbol == NULL || m_symbol == "";
}

void AtomicFormula::SetValue(bool value)
{
	m_value = value;
}

void AtomicFormula::NegValue()
{
	m_value = !m_value;
}

unsigned AtomicFormula::GetHash()
{
	return m_hash;
}

char * AtomicFormula::GetSymbol()
{
	return m_symbol;
}