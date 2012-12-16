#ifndef FormulaSetList_h
#define FormulaSetList_h

#include "../../../HilbertIncludes.h"
#include "IFormulaSet.h"
#include <list>

using std::list;

/*
*	Advantage of using list: if an item is added to
*	the list, a previously used iterator won't become
*	invalid.
*/
class FormulaSetList : public IFormulaSet
{
public:
	FormulaSetList() {}
	~FormulaSetList();

	void Add(IFormula * formula);
	void Add(spIFormula formula);
	void Add(IFormulaSet& fset);

	list<spIFormula>::iterator Begin();
	list<spIFormula>::iterator End();
	
	unsigned Size();
	bool Contains(long hash);
	spIFormula Get(long hash);
	void Clear();
	string ToString();

private:
	list<spIFormula> m_formulas;
};

#endif