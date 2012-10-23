#include "General.h"

#include "../Formula/IFormula.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/FormulaSet.h"

namespace General
{
	/*
	*	Modus ponens:
	*	Cut the left part of the impF if possible then return true, res will contain impF's right part
	*	else return false and res := null.
	*/
	bool MP(IFormula& f, ImplicationFormula& impF, IFormula * res)
	{
		//TODO
		return false;
	}

	/*
	*	Deduct f into sigma: the left part of f is put into sigma, res := f's right, return true
	*	else return false and res := null.
	*/
	bool Deduction(ImplicationFormula& f, FormulaSet& sigma, IFormula * res)
	{
		//TODO
		return false;
	}

	/*
	*	Unification algorithm.
	*	Unify a and b, if it was successful return true and put the unified formula in res,
	*	else return false and res := null.
	*/
	bool Unification(IFormula& a, IFormula& b, IFormula * res)
	{
		//TODO
		return false;
	}
}