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
	return false;
}

bool ImplicationFormula::Eval()
{
	return ( !m_left || m_right );
}

bool ImplicationFormula::Equals(IFormula * formula)
{
	//TODO
	return false;
}

char * ImplicationFormula::ToString()
{
	//TODO
	return "";
}

IFormula * ImplicationFormula::GetLeftSub()
{
	m_left;
}

IFormula * ImplicationFormula::GetRightSub()
{
	return m_right;
}