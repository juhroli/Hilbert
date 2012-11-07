#include "FormulaSetVector.h"
#include "../AFormulaTable.h"
#include "../FormulaWrapper.h"

using namespace AFormulaTable;

FormulaSetVector::~FormulaSetVector()
{
	Clear();
}

void FormulaSetVector::Add(IFormula * formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == __nullptr)
	{
		FormulaWrapper * wrap = dynamic_cast<FormulaWrapper*>(formula);
		m_formulaMap[hash] =
			(formula->IsAtomic() && wrap == __nullptr ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
	}
}

void FormulaSetVector::Add(spIFormula formula)
{
	AddFormula(formula);
}

void FormulaSetVector::Add(IFormulaSet& fset)
{
	//TODO
}

void FormulaSetVector::AddFormula(spIFormula formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == __nullptr)
	{
		FormulaWrapper * wrap = dynamic_cast<FormulaWrapper*>(formula.get());
		m_formulaMap[hash] =
			(formula->IsAtomic() && wrap == __nullptr ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
	}
}

/*
*	Sort formulas by their length.
*/
void FormulaSetVector::SortFormulas()
{
	parallel_radixsort(begin(m_formulas), end(m_formulas),
		[](spIFormula f) -> unsigned {
			return f->Length();
		});
}

vector<spIFormula>::iterator FormulaSetVector::Begin()
{
	return m_formulas.begin();
}

vector<spIFormula>::iterator FormulaSetVector::End()
{
	return m_formulas.end();
}

unsigned FormulaSetVector::Size()
{
	return m_formulas.size();
}

bool FormulaSetVector::Contains(long hash)
{
	return m_formulaMap[hash] != __nullptr;
}

spIFormula FormulaSetVector::Get(long hash)
{
	return m_formulaMap[hash];
}

void FormulaSetVector::Clear()
{
	m_formulaMap.clear();
	m_formulas.clear();
}