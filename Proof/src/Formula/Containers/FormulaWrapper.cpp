#include "FormulaWrapper.h"
#include <vector>

using std::vector;

FormulaWrapper::FormulaWrapper()
	: m_this(__nullptr)
	, m_origin(make_pair(__nullptr, __nullptr))
	, m_fromSigma(false)
{
}

FormulaWrapper::FormulaWrapper(IFormula * thisF)
	: m_this(thisF)
	, m_origin(make_pair(__nullptr, __nullptr))
	, m_fromSigma(true)
{
}

FormulaWrapper::FormulaWrapper(IFormula * thisF, replaces rep)
	: m_this(thisF)
	, m_origin(make_pair(__nullptr, __nullptr))
	, m_replaces(rep)
	, m_fromSigma(true)
{
}


FormulaWrapper::FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin)
	: m_this(thisF)
	, m_origin(origin)
	, m_fromSigma(false)
{
}

FormulaWrapper::FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin, replaces rep)
	: m_this(thisF)
	, m_origin(origin)
	, m_replaces(rep)
	, m_fromSigma(false)
{

}

FormulaWrapper::FormulaWrapper(FormulaWrapper& formula)
{
	/*
	m_this = spIFormula(formula.m_this);
	m_origin = make_pair(spIFormula(formula.m_origin.first), spIFormula(formula.m_origin.second));
	*/
}

FormulaWrapper::~FormulaWrapper()
{
	DELETEFORMULA(m_this);
}

bool FormulaWrapper::IsAtomic()
{
	return m_this->IsAtomic();
}

bool FormulaWrapper::IsTemp()
{
	return m_this->IsTemp();
}

bool FormulaWrapper::Eval()
{
	return m_this->Eval();
}

bool FormulaWrapper::Equals(IFormula * formula)
{
	return m_this->Equals(formula);
}

string FormulaWrapper::ToString()
{
	return m_this->ToString();
}

IFormula * FormulaWrapper::Clone()
{
	return new FormulaWrapper(*this);
}

bool FormulaWrapper::IsNull()
{
	return m_this->IsNull();
}

IFormula * FormulaWrapper::Replace(IFormula& t, IFormula& x)
{
	return m_this->Replace(t, x);
}

unsigned FormulaWrapper::Length()
{
	return m_this->Length();
}

long FormulaWrapper::HashCode()
{
	return m_this->HashCode();
}

pair<IFormula*, IFormula*> FormulaWrapper::GetOrigin()
{
	return m_origin;
}

bool FormulaWrapper::IsFromSigma()
{
	return m_fromSigma;
}

IFormula * FormulaWrapper::GetThis()
{
	return m_this;
}

string FormulaWrapper::GetReplaces()
{
	stringstream stream;

	for(auto it : m_replaces)
	{
		stream << "[" << it.first->ToString() << "/" << it.second->ToString() << "] ";
	}

	return stream.str();
}