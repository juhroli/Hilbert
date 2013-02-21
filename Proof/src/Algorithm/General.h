#ifndef General_h
#define General_h

#include <list>

class IFormula;
class ImplicationFormula;
class IFormulaSet;
class AlgorithmBase;

using std::list;
using std::pair;
using std::string;

/*
*	This namespace contains the basic functions on formulas, such as:
*	modus ponens, deduction, unification, replace all,
*	does formula contain a sub-formula, replaces normalizer,
*	formula normalizer, clear replaces.
*	Algorithms can be created with the CreateAlgorithm function, and
*	their descriptions can be returned by another function.
*/
namespace General
{
	enum AlgorithmType
	{
		ALG_0x00,
		ALG_0x01,
		ALG_0x02,
		ALG_0x03
	};

	typedef list<pair<IFormula*, IFormula*>> replaces;

	bool MP(IFormula * f, ImplicationFormula * impF, IFormula*& res);
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, replaces& uni);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res);
	IFormula * ReplaceAll(IFormula * formula, replaces& rep);
	bool ContainsFormula(IFormula * f, IFormula * g);
	void NormalizeReplaces(replaces& rep);
	void ClearReplaces(replaces& rep);
	void NormalizeFormula(IFormula*& f);

	AlgorithmBase * CreateAlgorithm(AlgorithmType type);
	AlgorithmBase * CreateAlgorithm(AlgorithmType type, unsigned parm);
	string AlgorithmsDescription();
}

#endif