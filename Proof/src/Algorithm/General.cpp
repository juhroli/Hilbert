#include "../HilbertDefines.h"
#include "General.h"

#include<functional>
#include "../Formula/IFormula.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "../Formula/Containers/AFormulaTable.h"
#include "AlgorithmBase.h"

/*	Put algorithm headers here.	*/
#include "Algorithm0x00.h"
#include "Algorithm0x01.h"
#include "Algorithm0x02.h"
#include "Algorithm0x03.h"
/*	===========================	*/

namespace General
{
	using namespace AFormulaTable;
	/*
	*	Modus ponens:
	*	Cut the left part of the impF if possible then return true, res will contain impF's right part
	*	else return false and res := null.
	*	res is an output parameter referencing to a pointer.
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
		{
			DELETEFORMULA(res);
		}

		return ret;
	}

	/*
	*	Deduct f into sigma: the left part of f is put into sigma, res := f's right, return true
	*	else return false and res := null.
	*	res is an output parameter referencing to a pointer.
	*/
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res)
	{
		if(f == nullptr || sigma == nullptr)
		{
			DELETEFORMULA(res);
			return false;
		}

		if(!f->IsAtomic())
		{
			ImplicationFormula * impF = static_cast<ImplicationFormula*>(f);
			sigma->Add(impF->GetLeftSub()->Clone());
			res = impF->GetRightSub()->Clone();
			return true;
		}

		DELETEFORMULA(res);

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
	*	res is an output parameter referencing to a pointer.
	*/
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, replaces& uni)
	{
		if(a == nullptr || b == nullptr)
		{
			ClearReplaces(uni);
			DELETEFORMULA(res);
			return false;
		}

		bool ret = true;

		if(a->Equals(b))
		{
			DELETEFORMULA(res);
			res = a->Clone();
			return true;
		}

		//If both are compound
		if(!a->IsAtomic() && !b->IsAtomic())
		{
			if(!a->IsTemp() && !b->IsTemp())
			{
				ClearReplaces(uni);
				DELETEFORMULA(res);
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
				IFormula * x = F;

				F = static_cast<ImplicationFormula*>(ReplaceAll(F, uni));

				if(F != x)
				{
					DELETEFORMULA(x);
				}
				
				x = G;
				G = static_cast<ImplicationFormula*>(ReplaceAll(G, uni));

				if(G != x)
				{
					DELETEFORMULA(x);
				}
			};

			//Do the unification on the left part of the implication
			if(ret = Unification(F->GetLeftSub(), G->GetLeftSub(), res, uni))
				replace();
			else
			{
				ClearReplaces(uni);
				DELETEFORMULA(res);
				DELETEFORMULA(G);
				DELETEFORMULA(F);
				return false;
			}

			//Do the unification on the right part of the implication
			if(ret = Unification(F->GetRightSub(), G->GetRightSub(), res, uni))
				replace();
			else
			{
				ClearReplaces(uni);
				DELETEFORMULA(res);
				DELETEFORMULA(G);
				DELETEFORMULA(F);
				return false;
			}

			//If the result F and G contains null, set ret to false and res to nullptr
			if(!(ret = !F->IsNull() && !G->IsNull()))
			{
				ClearReplaces(uni);
				DELETEFORMULA(res);
			}
			else if(ret = F->Equals(G))
			{
				DELETEFORMULA(res);
				res = F->Clone(); //If F equals G, the result is the clone of F
			}
			
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

				IFormula * x;
				x = res;
				res = G->Replace(G, F);
				if(res != x)
				{
					DELETEFORMULA(x);
				}
			}
			else
			{
				ClearReplaces(uni);
				DELETEFORMULA(res);
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
	*	res is an output parameter referencing to a pointer.
	*/
	bool Unification(IFormula * a, IFormula * b, IFormula*& res)
	{
		res = nullptr;

		if(a == nullptr || b == nullptr)
			return false;

		replaces uni;
		bool ret = Unification(a, b, res, uni);
		
		ClearReplaces(uni);

		return ret;
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

			IFormula * x = ret;

			ret = ret->Replace(it.first, it.second);

			if(ret != x)
			{
				DELETEFORMULA(x);
			}
		}

		return ret;
	}

	/*
	*	Create an algorithm instance.
	*/
	AlgorithmBase * CreateAlgorithm(AlgorithmType type)
	{
		switch(type)
		{
		case ALG_0x00:
			return new Algorithm0x00();
		case ALG_0x01:
			return new Algorithm0x01();
		case ALG_0x02:
			return new Algorithm0x02();
		case ALG_0x03:
			return new Algorithm0x03();
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
						DELETEFORMULA(it->second);
						it->second = it2->second->Clone();
					}

					--it3;

					if(it3 != it && ContainsFormula(it->second, it3->first))
					{
						IFormula * temp = it->second;
						it->second = it->second->Replace(it3->first, it3->second);

						if(temp != it->second)
						{
							DELETEFORMULA(temp);
						}
					}
					
					it2++;
				}
			}
			it++;
		}

		/*
		*	Erase duplications.
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

					DELETEFORMULA(erase->first);
					DELETEFORMULA(erase->second);
					rep.erase(erase);
				}
				else
					it2++;
			}
			it++;
		}
	}

	/*
	*	Deletes the formulas in the list.
	*/
	void ClearReplaces(replaces& rep)
	{
		for(auto it : rep)
		{
			DELETEFORMULA(it.first);
			DELETEFORMULA(it.second);
		}
		rep.clear();
	}

	/*
	*	Normalizes the input f formula by removing double negations.	
	*/
	void NormalizeFormula(IFormula*& f)
	{
		ImplicationFormula * impl = nullptr;

		if(!f->IsAtomic() && !f->IsWrapped())
			impl = static_cast<ImplicationFormula*>(f);
		else
			return;

		if( impl->GetRightSub()->Equals(GetAtomicFormula(FALSE).get()) )
		{
			if(!impl->GetLeftSub()->IsAtomic())
			{
				ImplicationFormula * leftImpl = static_cast<ImplicationFormula*>(impl->GetLeftSub());

				if( leftImpl->GetRightSub()->Equals(GetAtomicFormula(FALSE).get()) )
				{
					IFormula * oldF = f;
					f = leftImpl->GetLeftSub()->Clone();
					DELETEFORMULA(oldF);
				}
			}
		}

		impl = dynamic_cast<ImplicationFormula*>(f);
		
		if(impl != nullptr)
		{
			IFormula * leftNorm = impl->GetLeftSub()->Clone();
			IFormula * rightNorm = impl->GetRightSub()->Clone();

			NormalizeFormula(leftNorm);
			NormalizeFormula(rightNorm);

			DELETEFORMULA(f);

			f = new ImplicationFormula(leftNorm, rightNorm);
		}
	}

	/*
	*	Returns the algorithms' descriptions.
	*/
	string AlgorithmsDescription()
	{
		//TODO: write desc.
		stringstream stream;
		stream << "Algorithm arg id: " << ALG_0x00 << endl
			<< "\tdesc.: this algorithm does deduction on the target," << endl
			<< "\tputs the axioms into its set then iterates forward" << endl
			<< "\ton the set and tries to cut the formulas both ways with each other." << endl
			<< "\tThe algorithm will stop either if it found a result or not." << endl
			<< "\tReturning no results does not necessary mean that there is no proof," << endl
			<< "\tthere might be, but the algorithm couldn't find it." << endl

			<< "Algorithm arg id: " << ALG_0x01 << endl
			<< "\tdesc.: Implemented, but no desc, yet." << endl
			<< "Algorithm arg id: " << ALG_0x02 << endl
			<< "\tdesc.: Implemented, but no desc, yet." << endl

			<< "Algorithm arg id: " << ALG_0x03 << endl
			<< "\tdesc.: this algorithm does deduction on the target," << endl
			<< "\tputs the axioms into its set then chooses two formulas" << endl
			<< "\trandomly from its set and tries to cut the formulas" << endl
			<< "\tboth ways with each other." << endl
			<< "\tThe algorithm will stop either if it found a result or not." << endl
			<< "\tReturning no results does not necessary mean that there is no proof," << endl
			<< "\tthere might be, but the algorithm couldn't find it." << endl;

		return stream.str();
	}
}