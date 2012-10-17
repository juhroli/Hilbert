#include "FormulaSet.h"
#include "AFormulaTable.h"

using namespace AFormulaTable;

FormulaSet::~FormulaSet()
{
	formulaMap.clear();
	formulas.clear();
}

void FormulaSet::Add(IFormula * formula)
{
	AddFormula(formula);
}

void FormulaSet::AddFormula(IFormula * formula)
{
	long hash = formula->HashCode();

	if(formulaMap[hash] == __nullptr)
	{
		formulaMap[hash] =
			(!formula->IsAtomic() ? spIFormula(formula) : spIFormula(GetAtomicFormula(formula->HashCode())));
		formulas.push_back(formulaMap[hash]);
	}
}

/*
*	Sort formulas by their length.
*/
void FormulaSet::SortFormulas()
{
	parallel_radixsort(begin(formulas), end(formulas),
		[](spIFormula f) {
			return f->Length();
		});
}

vector<spIFormula>::iterator FormulaSet::Begin()
{
	return formulas.begin();
}

vector<spIFormula>::iterator FormulaSet::End()
{
	return formulas.end();
}

unsigned FormulaSet::Size()
{
	return formulas.size();
}
