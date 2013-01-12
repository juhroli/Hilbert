#ifndef FormulaGenerator_h
#define FormulaGenerator_h

#include "HilbertDefines.h"
#include "Formula/IFormula.h"
#include <vector>

using std::vector;

/*
*	Formulas are stored by bits.
*	Each double bit means a variable.
*	00: empty
*	01: true
*	10: false
*/
typedef long long formula;
typedef vector<formula> formulas;

class FormulaSetList;

/*
*	This class generates a logical consequence
*	{F1, ..., Fn} |= F only using {->, ~}.
*	The result string is compatible with the ProofUI's
*	file input format.
*/
class FormulaGenerator
{
public:
	FormulaGenerator();
	FormulaGenerator(unsigned n);
	~FormulaGenerator();

	string Generate();

private:
	unsigned m_N; //Number of max variables in a formula
	formula m_mask; //A mask to use in bit operations

	void GenStartSet(formula v, formulas& set, int n);
	formula GenContradiction(formula v);
	void Normalize(formula& v);
	formula Negate(formula v);
	IFormula * DisjunctFormulas(FormulaSetList flist);
	IFormula * ConvertToFormula(formula v, bool disjConn);
};

#endif