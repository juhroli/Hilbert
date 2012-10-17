#pragma once

#include "../IFormula.h"
#include "../Atomic/TempFormula.h"

class ImplicationFormula : public IFormula
{
public:
	ImplicationFormula();
	ImplicationFormula(IFormula * left, IFormula * right);
	ImplicationFormula(ImplicationFormula& formula);
	~ImplicationFormula();
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

	void SetLeftSub(IFormula * formula);
	void SetRightSub(IFormula * formula);
	IFormula * GetLeftSub();
	IFormula * GetRightSub();

	string ToStringPostfix();
	string ToStringPrefix();

protected:
	IFormula * m_left;
	IFormula * m_right;
	unsigned m_length;
	long m_hash;

	void GenerateHash();
};