#include "ImplicationFormula.h"

ImplicationFormula::ImplicationFormula()
	: IFormula()
{
	m_left = NULL;
	m_right = NULL;
}

ImplicationFormula::ImplicationFormula(IFormula * left, IFormula * right)
	: IFormula()
	, m_left(left)
	, m_right(right)
{
}

ImplicationFormula::ImplicationFormula(ImplicationFormula& formula)
{
	m_left = formula.GetLeftSub();
	m_right = formula.GetRightSub();
	if(m_left != NULL && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != NULL && !m_right->IsAtomic())
		m_right = m_right->Clone();
}

ImplicationFormula::~ImplicationFormula()
{
	if(m_left != NULL)
	{
		delete m_left;
		m_left = NULL;
	}
	if(m_right != NULL)
	{
		delete m_right;
		m_right = NULL;
	}
}

bool ImplicationFormula::IsAtomic()
{
	return false;
}

bool ImplicationFormula::IsTemp()
{
	return ( m_left->IsTemp() || m_right->IsTemp() );
}

bool ImplicationFormula::Eval()
{
	return ( !m_left || m_right );
}

bool ImplicationFormula::Equals(IFormula * formula)
{
	if(formula->IsAtomic())
	{
		return false;
	}

	return m_left->Equals(static_cast<ImplicationFormula*>(formula)->GetLeftSub()) 
		&& m_right->Equals(static_cast<ImplicationFormula*>(formula)->GetRightSub());
}

string ImplicationFormula::ToString()
{
	stringstream stream;

	if(!m_left->IsAtomic())
	{
		stream << "(";
		stream << m_left->ToString();
		stream << ")";
	}
	else
	{
		stream << m_left->ToString();
	}
	
	stream << IMPLIES;

	if(!m_right->IsAtomic())
	{
		stream << "(";
		stream << m_right->ToString();
		stream << ")";
	}
	else
	{
		stream << m_right->ToString();
	}

	return stream.str();
}

IFormula * ImplicationFormula::Clone()
{
	return new ImplicationFormula(*this);
}

bool ImplicationFormula::IsNull()
{
	return m_left == NULL || m_right == NULL;
}

void ImplicationFormula::SetLeftSub(IFormula * formula)
{
	m_left = formula;
}

void ImplicationFormula::SetRightSub(IFormula * formula)
{
	m_right = formula;
}

IFormula * ImplicationFormula::GetLeftSub()
{
	return m_left;
}

IFormula * ImplicationFormula::GetRightSub()
{
	return m_right;
}