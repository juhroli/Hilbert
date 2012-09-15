#pragma once

#include "IFormula.h"

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
	void SetLeftSub(IFormula * formula);
	void SetRightSub(IFormula * formula);
	IFormula * GetLeftSub();
	IFormula * GetRightSub();

protected:
	IFormula * m_left;
	IFormula * m_right;
};