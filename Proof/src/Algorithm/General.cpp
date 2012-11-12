#include "General.h"

#include<functional>
#include "../Formula/IFormula.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "IAlgorithm.h"

/*	Put algorithm headers here.	*/
#include "Algorithm0.h"
/*	===========================	*/

namespace General
{
	/*
	*	Modus ponens:
	*	Cut the left part of the impF if possible then return true, res will contain impF's right part
	*	else return false and res := null.
	*	res is an output parameter references a pointer.
	*/
	bool MP(IFormula * f, ImplicationFormula * impF, IFormula*& res)
	{
		if(f == __nullptr || impF == __nullptr)
		{
			res = __nullptr;
			return false;
		}

		bool ret;

		if(ret = f->Equals(impF->GetLeftSub()))
			res = impF->GetRightSub()->Clone();
		else
			res = __nullptr;

		return ret;
	}

	/*
	*	Deduct f into sigma: the left part of f is put into sigma, res := f's right, return true
	*	else return false and res := null.
	*	res is an output parameter references a pointer.
	*/
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res)
	{
		if(f == __nullptr || sigma == __nullptr)
		{
			res = __nullptr;
			return false;
		}

		if(!f->IsAtomic())
		{
			ImplicationFormula * impF = static_cast<ImplicationFormula*>(f);
			sigma->Add(impF->GetLeftSub()->Clone());
			res = impF->GetRightSub()->Clone();
			return true;
		}

		res = __nullptr;

		return false;
	}

	/*
	*	Unification algorithm.
	*	Unify a and b, if it was successful return true and put the unified formula in res,
	*	else return false and res := null.
	*
	*	Only formulas with temp atomic formulas can be unified with other formulas!
	*	
	*	uni will contain a sequence of replaces for the unification.
	*	res is an output parameter references a pointer.
	*/
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, replaces& uni)
	{
		if(a == __nullptr || b == __nullptr)
		{
			res = __nullptr;
			return false;
		}

		bool ret = true;

		if(a->Equals(b))
		{
			res = a->Clone();
			return true;
		}

		//If both are compound
		if(!a->IsAtomic() && !b->IsAtomic())
		{
			if(!a->IsTemp() && !b->IsTemp())
			{
				res = __nullptr;
				return false;
			}

			ImplicationFormula * F = static_cast<ImplicationFormula*>(a->Clone());
			ImplicationFormula * G = static_cast<ImplicationFormula*>(b->Clone());

			/*
			*	Lambda function for replace.
			*	Do the replaces in the replace list.
			*/
			auto replace = [&]()
			{
				for(auto it : uni)
				{
					F = static_cast<ImplicationFormula*>(F->Replace(it.first, it.second));
					G = static_cast<ImplicationFormula*>(G->Replace(it.first, it.second));
				}
			};

			//Do the unification on the left part of the implication
			if(ret = Unification(F->GetLeftSub(), G->GetLeftSub(), res, uni))
				replace();
			else
			{
				DELETEFORMULA(G);
				DELETEFORMULA(F);
				return false;
			}

			//Do the unification on the right part of the implication
			if(ret = Unification(F->GetRightSub(), G->GetRightSub(), res, uni))
				replace();
			else
			{
				DELETEFORMULA(G);
				DELETEFORMULA(F);
				return false;
			}

			//If the result F and G contains null, set ret to false and res to nullptr
			if(!(ret = !F->IsNull() && !G->IsNull()))
				res = __nullptr;
			else if(ret = F->Equals(G))
				res = F->Clone(); //If F equals G the result is the clone of F
			
			DELETEFORMULA(G);
			DELETEFORMULA(F);
		}
		else
		{
			//If at least one of them is an atomic temp
			IFormula * F = a;
			IFormula * G = b;
			if(F->IsAtomic() && F->IsTemp() && !G->IsTemp())
			{
				F = b;
				G = a;
			}

			if(G->IsTemp() && G->IsAtomic())
			{				
				//If F doesn't contain G
				if(ret = !ContainsFormula(F, G))
					uni.push_back(make_pair(G->Clone(), F->Clone()));
				res = G->Replace(G, F);
			}
			else
			{
				res = __nullptr;
				ret = false;
			}
		}

		return ret;
	}

	/*
	*	Unification algorithm.
	*	Unify a and b, if it was successful return true and put the unified formula in res,
	*	else return false and res := null.
	*
	*	Only formulas with temp atomic formulas can be unified with other formulas!
	*	
	*	res is an output parameter references a pointer.
	*/
	bool Unification(IFormula * a, IFormula * b, IFormula*& res)
	{
		res = __nullptr;

		if(a == __nullptr || b == __nullptr)
			return false;

		replaces uni;

		return Unification(a, b, res, uni);
	}

	/*
	*	Do all the replaces defined in "rep" on the formula.
	*/
	IFormula * ReplaceAll(IFormula * formula, replaces& rep)
	{
		IFormula * ret = formula->Clone();

		for(auto it : rep)
		{
			if(!ret->IsTemp())
				break;
			ret = ret->Replace(it.first, it.second);
		}

		return ret;
	}

	/*
	*	Create an algorithm instance.
	*/
	IAlgorithm * Create(AlgorithmType type)
	{
		switch(type)
		{
		case ALG_0:
			return new Algorithm0();
		}
		return __nullptr;
	}

	/*
	*	Check if the first formula contains the second one.
	*/
	bool ContainsFormula(IFormula * f, IFormula * g)
	{
		if(f->IsAtomic())
			return f->Equals(g);
		else if(f->Equals(g))
			return true;

		FormulaWrapper * wrap = dynamic_cast<FormulaWrapper*>(f);

		if(wrap != __nullptr)
			f = wrap->GetThis();

		return ContainsFormula(static_cast<ImplicationFormula*>(f)->GetLeftSub(), g)
			|| ContainsFormula(static_cast<ImplicationFormula*>(f)->GetRightSub(), g);
	};
}