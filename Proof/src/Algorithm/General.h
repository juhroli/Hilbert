#pragma once

#include <vector>

class IFormula;
class ImplicationFormula;
class FormulaSet;
class IAlgorithm;

using namespace std;

namespace General
{
	typedef vector<pair<IFormula*, IFormula*>> unification;

	bool MP(IFormula * f, ImplicationFormula * impF, IFormula*& res);
	bool Deduction(IFormula * f, FormulaSet& sigma, IFormula*& res);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res, unification& uni);
	bool Unification(IFormula * a, IFormula * b, IFormula*& res);

	enum AlgorithmType
	{
		ALG_0
	};

	IAlgorithm * Create(AlgorithmType type);
}