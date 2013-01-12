#include "../../HilbertDefines.h"
#include "Axiom.h"

Axiom::Axiom()
	: ImplicationFormula()
{
}

Axiom::Axiom(IFormula * left, IFormula * right)
	: ImplicationFormula(left, right)
{
}

Axiom::Axiom(Axiom& axiom)
{
	m_left = axiom.GetLeftSub();
	m_right = axiom.GetRightSub();

	if(m_left != nullptr && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != nullptr && !m_right->IsAtomic())
		m_right = m_right->Clone();

	m_length = axiom.m_length;
	m_hash = axiom.m_hash;
	this->m_string = axiom.m_string;
	this->m_hashString = axiom.m_hashString;
	this->m_temp = axiom.m_temp;
}

IFormula * Axiom::Clone()
{
	return new Axiom(*this);
}