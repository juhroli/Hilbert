#include "Axiom.h"
#include "TempFormula.h"
#include "AFormulaTable.h"
#include <stack>

using std::stack;
using namespace AFormulaTable;

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

Axiom::~Axiom()
{
}

IFormula * Axiom::Clone()
{
	return new Axiom(*this);
}

Axiom * Axiom::Replace(char * x, IFormula * t)
{
	IFormula * temp = GetAtomicFormula(x);
	if(temp->IsTemp())
	{
		return this->Replace(static_cast<TempFormula*>(temp), t);
	}
	return NULL;
}

Axiom * Axiom::Replace(TempFormula * x, IFormula * t)
{
	Axiom * ret = new Axiom(*this);
	IFormula ** it = NULL;
	stack<IFormula*> formStack;
	formStack.push(this);

	while(!formStack.empty())
	{
		*it = formStack.top();
		if((*it)->IsTemp() && (*it)->Equals(x))
		{
			*it = t;
		}
		formStack.pop();

		if(!(*it)->IsAtomic())
		{
			formStack.push(static_cast<ImplicationFormula*>(*it)->GetLeftSub());
			formStack.push(static_cast<ImplicationFormula*>(*it)->GetRightSub());
		}
	}

	return ret;
}