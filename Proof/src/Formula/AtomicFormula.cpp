#include "AtomicFormula.h"
#include <hash_map>

using std::hash;

AtomicFormula::AtomicFormula()
	: IFormula()
	, m_symbol(NULL)
	, m_value(false)
	, m_id(UINT_MAX)
{
}

AtomicFormula::AtomicFormula(char * symbol)
	: IFormula()
	, m_symbol(symbol)
	, m_value(true)
	, m_id(UINT_MAX)
{
}

AtomicFormula::AtomicFormula(AtomicFormula& formula)
{
	m_symbol = formula.GetSymbol();
	m_value = formula.Eval();
	m_id = formula.GetId();
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
	
	return m_id == static_cast<AtomicFormula*>(formula)->GetId();
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

unsigned AtomicFormula::GetId()
{
	return m_id;
}

void AtomicFormula::SetId(unsigned id)
{
	m_id = id;
}

char * AtomicFormula::GetSymbol()
{
	return m_symbol;
}