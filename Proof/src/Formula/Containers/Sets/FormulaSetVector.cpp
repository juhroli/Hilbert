#include "FormulaSetVector.h"
#include "../AFormulaTable.h"
#include "../FormulaWrapper.h"

#ifndef _MSC_VER
	#include <algorithm>
	using std::stable_sort;
#endif

using namespace AFormulaTable;

FormulaSetVector::~FormulaSetVector()
{
	this->Clear();
}
template<typename T>
void FormulaSetVector::AddFormula(T formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == nullptr)
	{
		m_formulaMap[hash] =
			(formula->IsAtomic() && !formula->IsWrapped() ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
	}
}

void FormulaSetVector::Add(IFormula * formula)
{
	AddFormula(formula);
}

void FormulaSetVector::Add(spIFormula formula)
{
	AddFormula(formula);
}

void FormulaSetVector::Add(IFormulaSet& fset)
{
	FormulaSetVector& fsetVec = dynamic_cast<FormulaSetVector&>(fset);
	
	if(&fsetVec != nullptr)
	{
		vector<spIFormula>::iterator end = fsetVec.End();
		for(auto it = fsetVec.Begin(); it != end; it++)
		{
			this->Add(*it);
		}
	}
}

/*
*	Sort formulas by their length.
*/
void FormulaSetVector::SortFormulas()
{
#ifdef _MSC_VER
	parallel_radixsort(begin(m_formulas), end(m_formulas),
		[](spIFormula f) -> unsigned {
			return f->Length();
		});
#else
	stable_sort(m_formulas.begin(), m_formulas.end(),
		[](spIFormula x, spIFormula y) -> bool
		{
			return x.get()->Length() < y.get()->Length();
		});
#endif
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
	return m_formulaMap[hash] != nullptr;
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

string FormulaSetVector::ToString()
{
	if(Size() == 0)
		return "Empty set";

	stringstream stream;

	unsigned end = Size();

	stream << "{ ";

	for(unsigned i = 0; i < end; i++)
	{
		stream << (*this)[i].get()->ToString();
		stream << ((i + 1 != end) ? ", " : "");
	}

	stream<<" }";

	return stream.str();
}

spIFormula FormulaSetVector::operator[](unsigned pos)
{
	if(pos >= Size())
		return nullptr;

	return m_formulas[pos];
}