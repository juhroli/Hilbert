#pragma once

#include <vector>

class IFormula;
class ImplicationFormula;
class IFormulaSet;
class IAlgorithm;

using namespace std;

namespace General
{
	enum AlgorithmType
	{
		ALG_0
	};

	typedef vector<pair<IFormula*, IFormula*>> replaces;

	bool MP(IFormula * f, ImplicationFormula * impF, IFormula*& res);
	bool Deduction(IFormula * f, IFormulaSet * sigma, IFormula*& res);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, replaces& uni);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res);
	IFormula * ReplaceAll(IFormula * formula, replaces& rep);
	bool ContainsFormula(IFormula * f, IFormula * g);

	IAlgorithm * Create(AlgorithmType type);
}