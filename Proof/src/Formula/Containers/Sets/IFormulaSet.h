#ifndef IFormulaSet_h
#define IFormulaSet_h

#include "../../../HilbertIncludes.h"
#include <unordered_map>
#include "../../IFormula.h"

using namespace std;

/*
*	Type of the formula sets.
*/
enum FSetType
{
	FSET_VEC,
	FSET_LIST
};

/*
*	Interface for formula set
*/
class IFormulaSet
{
public:
	virtual ~IFormulaSet() {}

	virtual void Add(IFormula * formula) = 0;
	virtual void Add(spIFormula formula) = 0;
	virtual void Add(IFormulaSet& fset) = 0;

	virtual unsigned Size() = 0;
	virtual bool Contains(long hash) = 0;
	virtual spIFormula Get(long hash) = 0;
	virtual void Clear() = 0;
	virtual string ToString() = 0;

protected:
	unordered_map<long, spIFormula> m_formulaMap;
};

#endif