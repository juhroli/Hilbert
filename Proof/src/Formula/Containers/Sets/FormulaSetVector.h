#ifndef FormulaSetVector_h
#define FormulaSetVector_h

#include "../../../HilbertIncludes.h"
#ifdef _MSC_VER
	#include <ppl.h>
	using namespace concurrency;
#endif
#include <vector>
#include "IFormulaSet.h"

	using std::vector;

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

#endif