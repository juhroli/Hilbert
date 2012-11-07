#pragma once

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
	FormulaWrapper(IFormula * thisF, replaces rep);
	FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin);
	FormulaWrapper(IFormula * thisF, pair<IFormula*, IFormula*> origin, replaces rep);
	FormulaWrapper(FormulaWrapper& formula);
	~FormulaWrapper();

	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	bool IsNull();
	IFormula * Replace(IFormula& t, IFormula& x);
	unsigned Length();
	long HashCode();
	
	pair<IFormula*, IFormula*> GetOrigin();
	bool IsFromSigma();
	IFormula * GetThis();
	string GetReplaces();
	
private:
	pair<IFormula*, IFormula*> m_origin; //the origin of m_this: the pair's second was cut with the first
	IFormula * m_this; //the wrapped formula
	replaces m_replaces; //the replaces used in creating m_this
	bool m_fromSigma; //m_this is from sigma?
};