#pragma once

#include <ppl.h>
#include <vector>
#include "IFormulaSet.h"

using namespace concurrency;

class FormulaSetVector : public IFormulaSet
{
public:
	FormulaSetVector() {}
	~FormulaSetVector();

	void Add(IFormula * formula);
	void Add(spIFormula formula);
	void Add(IFormulaSet& fset);

	void SortFormulas();

	vector<spIFormula>::iterator Begin();
	vector<spIFormula>::iterator End();
	
	unsigned Size();
	bool Contains(long hash);
	spIFormula Get(long hash);
	void Clear();

	vector<spIFormula>& GetVector()
	{
		return m_formulas;
	}

private:
	vector<spIFormula> m_formulas;

protected:
	void AddFormula(spIFormula formula);
};