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
	string NormalizeAxiomString(string str);

	AlgorithmBase * CreateAlgorithm(AlgorithmType type);
	string AlgorithmsDescription();
}

#endif