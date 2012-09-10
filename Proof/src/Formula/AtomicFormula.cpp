#include "AtomicFormula.h"
#include <iostream>

AtomicFormula::AtomicFormula()
	: IFormula()
{
	m_symbol = NULL;
}

AtomicFormula::AtomicFormula(char * symbol)
	: IFormula()
	, m_symbol(symbol)
	, m_value(true)
{
}

AtomicFormula::~AtomicFormula()
{
	//if(m_symbol != NULL)
	//{
		//delete m_symbol;
		m_symbol = NULL;
	//}
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

char * AtomicFormula::ToString()
{
	return m_symbol;
}


bool AtomicFormula::Equals(IFormula * formula)
{
	if(!formula->IsAtomic())
		return false;
	
	return ( std::strcmp(m_symbol, formula->ToString()) == 0 );
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