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
	if(m_left != NULL && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != NULL && !m_right->IsAtomic())
		m_right = m_right->Clone();
}

IFormula * Axiom::Clone()
{
	return new Axiom(*this);
}