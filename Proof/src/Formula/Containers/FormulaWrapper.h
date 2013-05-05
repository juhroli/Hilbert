#ifndef FormulaWrapper_h
#define FormulaWrapper_h

#include "../../HilbertDefines.h"
#include "../IFormula.h"
#include "../../Algorithm/General.h"

using std::pair;
using std::make_pair;
using General::replaces;
/*
*
*/
class FormulaWrapper : public IFormula
{
public:
	FormulaWrapper();
	FormulaWrapper(IFormula * thisF);
	FormulaWrapper(IFormula * thisF, replaces& rep);
	FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin);
	FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin, replaces& rep);
	FormulaWrapper(FormulaWrapper& formula);
	~FormulaWrapper();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	IFormula * Replace(IFormula * x, IFormula * t);
	unsigned Length();
	long HashCode();
	bool IsWrapped();
	
	pair<IFormula*, IFormula*> GetOrigin();
	bool IsFromSigma();
	bool IsAxiom();
	IFormula * GetThis();
	string GetReplacesString();
	void AddReplaces(replaces rep);
	replaces& GetReplaces();
	
private:
	pair<IFormula*, IFormula*> m_origin; //the origin of m_this: the pair's second was cut with the first
	IFormula * m_this; //the wrapped formula
	replaces m_replaces; //the replaces used in creating m_this
	bool m_fromSigma; //is m_this from sigma?
	bool m_isAxiom; //is m_this an axiom?
	long m_hash;
};

#endif