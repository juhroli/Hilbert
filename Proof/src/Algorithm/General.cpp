#include "../HilbertIncludes.h"
#include "General.h"

#include<functional>
#include "../Formula/IFormula.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "AlgorithmBase.h"

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
		if(f == nullptr || impF == nullptr)
		{
			res = nullptr;
			return false;
		}

		bool ret;

		if(ret = f->Equals(impF->GetLeftSub()))
			res = impF->GetRightSub()->Clone();
		else
			res = nullptr;

		return ret;
	}

	/*
	*	Deduct f into sigma: the left part of f is put into sigma, res := f's right, return true
	*	else return false and res := null.
	*	res is an output parameter references a pointer.
	*/
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res)
	{
		if(f == nullptr || sigma == nullptr)
		{
			res = nullptr;
			return false;
		}

		if(!f->IsAtomic())
		{
			ImplicationFormula * impF = static_cast<ImplicationFormula*>(f);
			sigma->Add(impF->GetLeftSub()->Clone());
			res = impF->GetRightSub()->Clone();
			return true;
		}

		res = nullptr;

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
		if(a == nullptr || b == nullptr)
		{
			res = nullptr;
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
				res = nullptr;
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
				res = nullptr;
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
				res = nullptr;
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
		res = nullptr;

		if(a == nullptr || b == nullptr)
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
	AlgorithmBase * Create(AlgorithmType type)
	{
		switch(type)
		{
		case ALG_0:
			return new Algorithm0();
		}
		return nullptr;
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

		if(wrap != nullptr)
			f = wrap->GetThis();

		return ContainsFormula(static_cast<ImplicationFormula*>(f)->GetLeftSub(), g)
			|| ContainsFormula(static_cast<ImplicationFormula*>(f)->GetRightSub(), g);
	};

	/*
	*	Normalize replaces:
	*	If there is a [a/c] [c/F] like replace, where
	*	a and c are temps, and F is a formula then
	*	transform it into: [a/F] [c/F].
	*	And checks if rep has a
	*	[a/b->G] like and a [b/F] like replace
	*	and then transforms it into [a/F->G].
	*/
	void NormalizeReplaces(replaces& rep)
	{
		if(rep.size() == 1)
			return;

		replaces::iterator it = rep.begin();
		while(it != rep.end())
		{
			if(it->second->IsTemp())
			{
				replaces::iterator it2 = rep.begin();
				replaces::iterator it3 = rep.end();
				while(it2 != rep.end() && it3 != rep.begin())
				{
					//Normalize the chain replaces
					if(it != it2 && it->second->Equals(it2->first))
					{
						it->second = it2->second->Clone();
					}

					--it3;

					if(it3 != it && ContainsFormula(it->second, it3->first))
					{
						it->second = it->second->Replace(it3->first, it3->second);
					}
					
					it2++;
				}
			}
			it++;
		}

		/*
		*	Erase multiple replaces.
		*	Example: [a/F] [b/G] [a/H] will be
		*	[a/F] [b/G].
		*/
		it = rep.begin();
		while(it != rep.end())
		{
			replaces::iterator it2 = rep.begin();
			while(it2 != rep.end())
			{
				if(it != it2 && it->first->Equals(it2->first))
				{
					auto erase = it2;
					if(it2 != rep.end())
						it2++;
					rep.erase(erase);
				}
				else
					it2++;
			}
			it++;
		}
	}
}