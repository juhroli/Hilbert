#include "FormulaWrapper.h"

#include "../../Algorithm/General.h"

using General::ContainsFormula;

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
	if( (m_isAxiom = thisF->IsTemp() && !thisF->IsAtomic()) )
		m_fromSigma = false;

	m_hash = m_this->HashCode();
}

FormulaWrapper::FormulaWrapper(IFormula * thisF, replaces rep)
	: m_this(thisF)
	, m_origin(make_pair(__nullptr, __nullptr))
	, m_replaces(rep)
	, m_fromSigma(true)
{
	if( (m_isAxiom = thisF->IsTemp() && !thisF->IsAtomic()) )
		m_fromSigma = false;

	stringstream stream;
	stream << m_this->ToString() << GetReplacesString();
	m_hash = GenerateHashCode(stream.str());
}


FormulaWrapper::FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin)
	: m_this(thisF)
	, m_origin(origin)
	, m_fromSigma(false)
	, m_isAxiom(false)
{
	m_hash = m_this->HashCode();
}

FormulaWrapper::FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin, replaces rep)
	: m_this(thisF)
	, m_origin(origin)
	, m_replaces(rep)
	, m_fromSigma(false)
	, m_isAxiom(false)
{
	stringstream stream;
	stream << m_this->ToString() << GetReplacesString();
	m_hash = GenerateHashCode(stream.str());
}

FormulaWrapper::FormulaWrapper(FormulaWrapper& formula)
{
	m_this = formula.m_this->Clone();
	if(!formula.IsFromSigma() && !formula.IsAxiom())
		m_origin = make_pair(formula.m_origin.first->Clone(), formula.m_origin.second->Clone());
	AddReplaces(formula.m_replaces);
	m_fromSigma = formula.IsFromSigma();
	m_isAxiom = formula.IsAxiom();
	m_hash = formula.m_hash;
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

IFormula * FormulaWrapper::Replace(IFormula * t, IFormula * x)
{
	return m_this->Replace(t, x);
}

unsigned FormulaWrapper::Length()
{
	return m_this->Length();
}

long FormulaWrapper::HashCode()
{
	return m_hash;
}

pair<IFormula*, IFormula*> FormulaWrapper::GetOrigin()
{
	return m_origin;
}

bool FormulaWrapper::IsFromSigma()
{
	return m_fromSigma;
}

bool FormulaWrapper::IsAxiom()
{
	return m_isAxiom;
}

IFormula * FormulaWrapper::GetThis()
{
	return m_this;
}

string FormulaWrapper::GetReplacesString()
{
	stringstream stream;

	for(auto it : m_replaces)
	{
		if(!ContainsFormula(m_this, it.first))
			continue;
		stream << "[" << it.first->ToString() << "/" << it.second->ToString() << "] ";
	}

	return stream.str();
}

/*
*	Adds the replaces to the member.
*/
void FormulaWrapper::AddReplaces(replaces rep)
{
	replaces::iterator it = rep.begin();
	while(it != rep.end())
	{
		m_replaces.push_back(make_pair(it->first->Clone(), it->second->Clone()));
		it++;
	}
}

replaces& FormulaWrapper::GetReplaces()
{
	return m_replaces;
}