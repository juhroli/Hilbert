#pragma once

#include "IFormula.h"

class ImplicationFormula : public IFormula
{
public:
	ImplicationFormula();
	ImplicationFormula(IFormula * left, IFormula * right);
	~ImplicationFormula();
	bool IsAtomic();
	bool IsTemp();
	bool Eval();
	bool Equals(IFormula * formula);
	char * ToString();
	virtual IFormula * GetLeftSub();
	virtual IFormula * GetRightSub();
	//int Count();

protected:
	IFormula * m_left;
	IFormula * m_right;
};