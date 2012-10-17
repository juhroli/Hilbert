#pragma once

#include <ppl.h>
#include <unordered_map>
#include <vector>
#include "../IFormula.h"
#include "../Compound/ImplicationFormula.h"

using namespace std;
using namespace concurrency;

class FormulaSet
{
public:
	FormulaSet() {}
	~FormulaSet();

	void Add(IFormula * formula);

	void SortFormulas();

	vector<spIFormula>::iterator Begin();
	vector<spIFormula>::iterator End();
	unsigned Size();

private:
	vector<spIFormula> formulas;

protected:
	unordered_map<long, spIFormula> formulaMap;

	void AddFormula(IFormula * formula);
};