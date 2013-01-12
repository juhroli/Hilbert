#ifndef FormulaSetVector_h
#define FormulaSetVector_h

#include "../../../HilbertDefines.h"
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
	string ToString();
	spIFormula operator[](unsigned pos);
	
private:
	vector<spIFormula> m_formulas;

	template<typename T>
	void AddFormula(T formula);
};

#endif