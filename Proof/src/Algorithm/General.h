#pragma once

class IFormula;
class ImplicationFormula;
class FormulaSet;
class IAlgorithm;

namespace General
{
	bool MP(IFormula& f, ImplicationFormula& impF, IFormula * res);
	bool Deduction(ImplicationFormula& f, FormulaSet& sigma, IFormula * res);
	bool Unification(IFormula& a, IFormula& b, IFormula * res);

	enum AlgorithmType
	{
		ALG_0
	};

	IAlgorithm * Create(AlgorithmType type);
}