#include "IAlgorithm.h"

using General::Unification;
using General::MP;
using General::ReplaceAll;

/*
*	This function does modus ponens, there and back. (The Hobbit function)
*	a: the formula from the outer loop
*	aImpl: a casted to ImplicationFormula
*	aLeft: if a is an ImplicationFormula this is it's left subformula
*	aWrapper: if a is a FromulaWrapper, then it is casted to that
*	returns true if the target formula was found
*/
bool IAlgorithm::MPBothWays(IFormula * a, IFormula * b)
{
	/* ===== Initialization ===== */
	ImplicationFormula * bImpl = __nullptr;
	IFormula * bLeft = __nullptr;
	FormulaWrapper * bWrapper = dynamic_cast<FormulaWrapper*>(b);

	if(bWrapper != __nullptr)
	{
		b = bWrapper->GetThis();
	}

	if(!b->IsAtomic())
	{
		bImpl = static_cast<ImplicationFormula*>(b);
		bLeft = bImpl->GetLeftSub();
	}

	ImplicationFormula * aImpl = __nullptr;
	IFormula * aLeft = __nullptr;
	FormulaWrapper * aWrapper = dynamic_cast<FormulaWrapper*>(a);

	if(aWrapper != __nullptr)
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
				m_sigma->Add(wrap);
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
			IFormula * unified = __nullptr;
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
					addWrap(y.second == __nullptr ? y.first : y.second, x.second == __nullptr ? x.first : x.second, res, uni);
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

void IAlgorithm::SetMaxLength(unsigned length)
{
	m_maxLength = length;
}