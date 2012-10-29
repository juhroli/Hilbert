#include "General.h"

#include<functional>
#include "../Formula/IFormula.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/FormulaSet.h"
#include "IAlgorithm.h"

/*	Put algorithm headers here.	*/

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
	bool Deduction(IFormula * f, FormulaSet& sigma, IFormula*& res)
	{
		if(!f->IsAtomic())
		{
			ImplicationFormula * impF = static_cast<ImplicationFormula*>(f);
			sigma.Add(impF->GetLeftSub()->Clone());
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
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, unification& uni)
	{
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
					F = static_cast<ImplicationFormula*>(F->Replace(*it.first, *it.second));
					G = static_cast<ImplicationFormula*>(G->Replace(*it.first, *it.second));
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
			if(F->IsAtomic() && !G->IsTemp() && F->IsTemp())
			{
				F = b;
				G = a;
			}

			if(G->IsTemp() && G->IsAtomic())
			{
				//Lambda function to check if the first formula contains the second one
				std::function<bool (IFormula *, IFormula *)> contains = [&] (IFormula * f, IFormula * g) -> bool
				{
					if(f->IsAtomic())
						return f->Equals(g);
				    
					return contains(static_cast<ImplicationFormula*>(f)->GetLeftSub(), g)
						|| contains(static_cast<ImplicationFormula*>(f)->GetRightSub(), g);
				};
				
				//If F doesn't contain G
				if(ret = !contains(F, G))
					uni.push_back(make_pair(G, F));
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
		unification uni;

		return Unification(a, b, res, uni);
	}

	/*
	*	Create an algorithm instance.
	*/
	IAlgorithm * Create(AlgorithmType type)
	{
		return __nullptr;
	}
}