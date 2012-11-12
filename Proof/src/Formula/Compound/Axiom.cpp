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

	if(m_left != __nullptr && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != __nullptr && !m_right->IsAtomic())
		m_right = m_right->Clone();

	m_length = axiom.Length();
	m_hash = axiom.HashCode();
	this->m_string = axiom.ToString();
	this->m_temp = axiom.IsTemp();
}

IFormula * Axiom::Clone()
{
	return new Axiom(*this);
}