#include "AlgorithmBase.h"
#include "General.h"
#include<functional>

using namespace General;

/*
*	This function does modus ponens, there and back. (The Hobbit function)
*
*	fset: put the result in this set
*	x in {a, b}
*	x: a formula
*	xImpl: x casted to ImplicationFormula
*	xLeft: if x is an ImplicationFormula this is it's left subformula
*	xWrapper: if x is a FromulaWrapper, then it is casted to that
*
*	returns true if the target or a temp atomic formula was found
*/
bool AlgorithmBase::MPBothWays(IFormula * a, IFormula * b, IFormulaSet * fset)
{
	if(a == nullptr || b == nullptr || fset == nullptr)
		return false;

	/* ===== Initialization ===== */
	ImplicationFormula * bImpl = nullptr;
	IFormula * bLeft = nullptr;
	FormulaWrapper * bWrapper = dynamic_cast<FormulaWrapper*>(b);

	if(bWrapper != nullptr)
	{
		b = bWrapper->GetThis();
	}

	if(!b->IsAtomic())
	{
		bImpl = static_cast<ImplicationFormula*>(b);
		bLeft = bImpl->GetLeftSub();
	}

	ImplicationFormula * aImpl = nullptr;
	IFormula * aLeft = nullptr;
	FormulaWrapper * aWrapper = dynamic_cast<FormulaWrapper*>(a);

	if(aWrapper != nullptr)
	{
		a = aWrapper->GetThis();
	}

	if(!a->IsAtomic())
	{
		aImpl = static_cast<ImplicationFormula*>(a);
		aLeft = aImpl->GetLeftSub();
	}
	/* ===== End of initialization ===== */

	/*
	*	This lambda function creates a FormulaWrapper instance from
	*	the given formulas, then puts it into sigma.
	*/
	std::function<void (IFormula*, IFormula*, IFormula*, replaces)> addWrap =
		[&](IFormula * a, IFormula * b, IFormula * c, replaces rep)
		{
			FormulaWrapper * wrap = new FormulaWrapper(c,
				make_pair(a, b), rep);

			if(wrap->Length() <= m_maxLength && !m_sigma->Contains(wrap->HashCode()))
			{
				fset->Add(wrap);
				m_last = wrap;
			}
			else
			{
				delete wrap;
			}
		};

	/*
	*	This lambda function unifies x's left sub formula with y,
	*	then cuts x with y.
	*/
	std::function<bool (pair<IFormula *, FormulaWrapper*>, ImplicationFormula*, IFormula*, pair<IFormula*, FormulaWrapper*>)> uniMP =
		[&] (pair<IFormula*, FormulaWrapper*> x, ImplicationFormula * xImpl, IFormula * xLeft, pair<IFormula*, FormulaWrapper*> y) -> bool
		{
			IFormula * unified = nullptr;
			replaces uni;

			if(Unification(xLeft, y.first, unified, uni))
			{
				//Do the replaces on x.
				if(xImpl->IsTemp())
					xImpl = dynamic_cast<ImplicationFormula*>(ReplaceAll(xImpl, uni));
				IFormula * res;
				//Cut with the unified
				if(MP(unified, xImpl, res))
				{
					addWrap(y.second == nullptr ? y.first : y.second, x.second == nullptr ? x.first : x.second, res, uni);
					if(m_last->Equals(m_target) || (m_last->IsTemp() && m_last->IsAtomic()))
					{
						return m_finished = true;
					}
				}
			}
			return false;
		};

	/* Cut both way. */
	return uniMP(make_pair(a, aWrapper), aImpl, aLeft, make_pair(b, bWrapper))
		|| uniMP(make_pair(b, bWrapper), bImpl, bLeft, make_pair(a, aWrapper));
}

void AlgorithmBase::SetMaxLength(unsigned length)
{
	m_maxLength = length;
}