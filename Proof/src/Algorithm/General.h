#ifndef General_h
#define General_h

#include <list>

class IFormula;
class ImplicationFormula;
class IFormulaSet;
class AlgorithmBase;

using namespace std;

namespace General
{
	enum AlgorithmType
	{
		ALG_0
	};

	typedef list<pair<IFormula*, IFormula*>> replaces;

	bool MP(IFormula * f, ImplicationFormula * impF, IFormula*& res);
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, replaces& uni);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res);
	IFormula * ReplaceAll(IFormula * formula, replaces& rep);
	bool ContainsFormula(IFormula * f, IFormula * g);
	void NormalizeReplaces(replaces& rep);

	AlgorithmBase * Create(AlgorithmType type);
}

#endif