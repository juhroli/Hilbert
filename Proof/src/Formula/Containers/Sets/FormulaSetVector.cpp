#include "FormulaSetVector.h"
#include "../AFormulaTable.h"
#include "../FormulaWrapper.h"

using namespace AFormulaTable;

FormulaSetVector::~FormulaSetVector()
{
	this->Clear();
}

void FormulaSetVector::Add(IFormula * formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == nullptr)
	{
		FormulaWrapper * wrap = dynamic_cast<FormulaWrapper*>(formula);
		m_formulaMap[hash] =
			(formula->IsAtomic() && wrap == nullptr ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
	}
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

void FormulaSetVector::AddFormula(spIFormula formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == nullptr)
	{
		FormulaWrapper * wrap = dynamic_cast<FormulaWrapper*>(formula.get());
		m_formulaMap[hash] =
			(formula->IsAtomic() && wrap == nullptr ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
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
		stream << ((i != end) ? ", " : "");
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