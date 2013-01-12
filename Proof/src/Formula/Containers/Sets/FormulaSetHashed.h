#ifndef FormulaSetPriority_h
#define FormulaSetPriority_h

#include "../../../HilbertDefines.h"
#include "IFormulaSet.h"
#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

class FormulaSetHashed : public IFormulaSet
{
public:
	FormulaSetHashed()
		: m_maxLength(0)
	{}
	~FormulaSetHashed();

	void Add(IFormula * formula);
	void Add(spIFormula formula);
	void Add(IFormulaSet& fset);
	
	unsigned Size();
	bool Contains(long hash);
	spIFormula Get(long hash);
	void Clear();
	string ToString();
	vector<spIFormula> operator[](unsigned pos);
	
private:
	/*
	*	A map hashed by the formulas length in the list.
	*/
	unordered_map< unsigned, vector<spIFormula> > m_formulas;
	unsigned m_maxLength;

	template<typename T>
	void AddFormula(T formula);
};

#endif