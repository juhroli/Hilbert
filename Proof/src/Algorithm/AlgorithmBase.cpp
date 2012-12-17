#include "AlgorithmBase.h"

#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetList.h"

#include<functional>
#include <iostream>

using namespace General;

AlgorithmBase::AlgorithmBase()
	: m_axioms(nullptr)
		, m_mpResults(nullptr)
		, m_target(nullptr)
		, m_sigma(nullptr)
		, m_last(nullptr)
		, m_result("")
		, m_finished(false)
		, m_maxLength(20)
{
}

void AlgorithmBase::SetAxioms(AxiomContainer * container)
{
    m_axioms = container;
}

bool AlgorithmBase::IsFinished()
{
	return m_finished;
}

void AlgorithmBase::SetMaxLength(unsigned length)
{
	m_maxLength = length;
}

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
bool AlgorithmBase::MPBothWays(IFormula * a, IFormula * b, IFormulaSet*& fset)
{
	if(a == nullptr || b == nullptr || fset == nullptr)
		return false;

	/*
	*	This lambda function creates a FormulaWrapper instance from
	*	the given formulas, then puts it into sigma.
	*/
	std::function<void (IFormula*, IFormula*, IFormula*, replaces&)> addWrap =
		[&](IFormula * f, IFormula * g, IFormula * h, replaces& rep)
		{
			FormulaWrapper * wrap = new FormulaWrapper(h,
				make_pair(f, g), rep);

			if(wrap->Length() <= m_maxLength && !fset->Contains(wrap->HashCode()))
			{
				fset->Add(wrap);

				if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
				{
					DELETEFORMULA(m_last);
				}

				m_last = wrap;
			}
			else
			{
				delete wrap;
				wrap = nullptr;
				h = nullptr;
			}
		};

	/*
	*	This lambda function unifies x's left sub formula with y,
	*	then cuts x with y.
	*/
	std::function<bool (IFormula*, IFormula*)> uniMP =
		[&] (IFormula * x, IFormula * y) -> bool
		{
			/* ===== Initialization ===== */
			ImplicationFormula * xImpl = nullptr;
			IFormula * xLeft = nullptr;
			FormulaWrapper * xWrapper = nullptr;

			if(x->IsWrapped())
			{
				xWrapper = dynamic_cast<FormulaWrapper*>(x);
				x = xWrapper->GetThis();
			}

			if(!x->IsAtomic())
			{
				xImpl = static_cast<ImplicationFormula*>(x);
				xLeft = xImpl->GetLeftSub();
			}

			ImplicationFormula * yImpl = nullptr;
			IFormula * yLeft = nullptr;
			FormulaWrapper * yWrapper = nullptr;

			if(y->IsWrapped())
			{
				yWrapper = dynamic_cast<FormulaWrapper*>(y);
				y = yWrapper->GetThis();
			}

			if(!b->IsAtomic())
			{
				yImpl = static_cast<ImplicationFormula*>(y);
				yLeft = yImpl->GetLeftSub();
			}
			/* ===== End of initialization ===== */

			IFormula * unified = nullptr;
			replaces uni;
			bool xImplDel = false;

			if(Unification(xLeft, y, unified, uni))
			{
				Stat_incUnificationCount();

				//Do the replaces on x.
				if(xImpl->IsTemp())
				{
					IFormula * tImpl = xImpl;
					//Apply the replaces on the x's whole formula
					xImpl = dynamic_cast<ImplicationFormula*>(ReplaceAll(xImpl, uni));

					if(tImpl != xImpl)
						xImplDel = true;
				}
				IFormula * res = nullptr;
				//Cut with the unified
				if(MP(unified, xImpl, res))
				{
					Stat_incMPCount();

					addWrap(yWrapper == nullptr ? y : yWrapper, xWrapper == nullptr ? x : xWrapper, res, uni);

					//Axioms count
					if(!x->IsWrapped() && !x->IsAtomic() && x->IsTemp()
						|| !y->IsWrapped() && !y->IsAtomic() && y->IsTemp())
						Stat_incAxiomCount();

					if(m_last->Equals(m_target) || (m_last->IsTemp() && m_last->IsAtomic()))
					{
						DELETEFORMULA(unified);

						if(xImplDel)
						{
							DELETEFORMULA(xImpl);
						}

						return m_finished = true;
					}
				}
				else
				{
					DELETEFORMULA(res);
					ClearReplaces(uni);
				}
			}

			DELETEFORMULA(unified);

			if(xImplDel)
			{
				DELETEFORMULA(xImpl);
			}

			return false;
		};

	/* Cut both ways. */
	return uniMP(a, b) || uniMP(b, a);
}

/*
*	Constructs the proof.
*
*/
string AlgorithmBase::ResultString()
{
	if(!m_finished)
		return "";

	if(!m_resString.empty())
		return m_resString;

	stringstream stream;

	stream<<"The proof is:"<<endl;
	FormulaSetList fset;
	
	/*
	*	First, create fset, which will contain
	*	the formulas in the proof.
	*	These formmulas are converted into FormulaWrappers.
	*/
	std::function<void (IFormula*)> add =
		[&] (IFormula * a)
		{
			FormulaWrapper * F = dynamic_cast<FormulaWrapper*>(a);
			FormulaWrapper * first = nullptr;
			FormulaWrapper * second = nullptr;
			FormulaWrapper * wrap = nullptr;

			replaces rep;
			if(a->IsAtomic() && a->IsTemp())
			{
				rep.push_back(make_pair(F->GetThis()->Clone(), m_target->Clone()));
			}

			if(F != nullptr)
			{
				F->AddReplaces(rep);

				/*
				*	Lambda function for creating the new origin for the formula.
				*/
				auto NewOrigin = [&](FormulaWrapper*& origin, IFormula * parent)
				{
					if(parent != nullptr)
					{
						
						if(parent->IsWrapped())
							origin = dynamic_cast<FormulaWrapper*>(parent);
						else
							origin = new FormulaWrapper(parent->Clone());

						if(origin->IsAxiom() || !origin->IsFromSigma())
							origin->AddReplaces(F->GetReplaces());

						if(!fset.Contains(origin->HashCode()))
							add(origin);

						else if(origin != parent)
						{
							unsigned hash = origin->HashCode();
							DELETEFORMULA(origin);
							origin = dynamic_cast<FormulaWrapper*>(fset.Get(hash).get());
						}
					}
				};

				NewOrigin(first, F->GetOrigin().first);
				NewOrigin(second, F->GetOrigin().second);

				if(first != nullptr && second != nullptr)
					wrap = new FormulaWrapper(F->GetThis()->Clone(), make_pair(static_cast<IFormula*>(first),
						static_cast<IFormula*>(second)));
				else
					wrap = (F == m_last ? dynamic_cast<FormulaWrapper*>(m_last->Clone()) : F);

				wrap->AddReplaces(F->GetReplaces());
			}
			else
			{
				wrap = new FormulaWrapper(a->Clone());
				wrap->AddReplaces(rep);
				ClearReplaces(rep);
			}

			fset.Add(wrap);
		};

	//Add the formulas into a FormulaSet -> duplications can be avoided
	add(m_last);
	
	int i = 1;

	//Write the steps into the stream
	for(auto it = fset.Begin(); it != fset.End(); it++, i++)
	{
		FormulaWrapper * F = dynamic_cast<FormulaWrapper*>(it->get());

		//First, normalize the replaces
		NormalizeReplaces(F->GetReplaces());

		stream<<i<<". ";
		if(F->IsFromSigma())
			stream<<F->ToString()<<"    from Sigma"<<endl<<endl;
		else
		{
			IFormula * replaced = ReplaceAll(F, F->GetReplaces());
			if(F->IsAxiom())
				stream<<replaced->ToString()<<"    Axiom: "<<F->ToString()<<" "
					<<F->GetReplacesString()<<endl<<endl;
			else if(!F->Equals(m_last) || !F->IsTemp())
			{
				IFormula * first = ReplaceAll(F->GetOrigin().first, F->GetReplaces());
				IFormula * second = ReplaceAll(F->GetOrigin().second, F->GetReplaces());
				stream<<replaced->ToString()<<"    : Cut "
					<<second->ToString()
					<<" with "<<first->ToString()<<endl<<endl;

				if(first != F->GetOrigin().first)
				{
					DELETEFORMULA(first);
				}

				if(second != F->GetOrigin().second)
				{
					DELETEFORMULA(second);
				}
			}

			if(F != replaced)
			{
				DELETEFORMULA(replaced);
			}
		}
	}
	fset.Clear();
	return m_resString = stream.str();
}

void AlgorithmBase::DoDeduction()
{
	IFormula * deduct = m_target;
	IFormula * res = nullptr;

	while(Deduction(deduct, m_sigma, res))
	{
		if(deduct != nullptr)
			delete deduct;
		m_target = res;
		deduct = m_target;
		res = nullptr;
	}

	deduct = nullptr;
}

void AlgorithmBase::AddAxiomsToSigma()
{
	for(int i = 1; i <= m_axioms->Size(); i++)
	{
		m_sigma->Add(m_axioms->GetAxiom(i));
	}
}