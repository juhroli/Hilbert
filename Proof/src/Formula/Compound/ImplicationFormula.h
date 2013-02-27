#ifndef ImplicationFormula_h
#define ImplicationFormula_h

#include "../IFormula.h"

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
	IFormula * Replace(IFormula * t, IFormula * x);
	unsigned Length();
	long HashCode();
	bool IsWrapped();
	string GetHashString();

	IFormula * GetLeftSub();
	IFormula * GetRightSub();

protected:
	IFormula * m_left;
	IFormula * m_right;
	unsigned m_length;
	long m_hash;
	string m_string;
	string m_hashString;

	enum triState
	{
		False,
		True,
		None
	};

	triState m_temp;
};

#endif