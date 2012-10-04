#pragma once

#include "IFormula.h"
#include "TempFormula.h"
#include "AFormulaTable.h"

class ImplicationFormula : public IFormula
{
public:
	ImplicationFormula();
	ImplicationFormula(IFormula * left, IFormula * right);
	ImplicationFormula(ImplicationFormula& formula);
	virtual ~ImplicationFormula();
	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	string ToString();
	IFormula * Clone();
	bool IsNull();
	IFormula * Replace(IFormula& t, IFormula& x);

	void SetLeftSub(IFormula * formula);
	void SetRightSub(IFormula * formula);
	IFormula * GetLeftSub();
	IFormula * GetRightSub();
	ImplicationFormula * Replace2(char * x, IFormula * t); //Replace the implication formula's temp atomic formula x in every occourance with t
	ImplicationFormula * Replace2(TempFormula * x, IFormula * t);

protected:
	IFormula * m_left;
	IFormula * m_right;
};