#include "FormulaSetHashed.h"
#include "../AFormulaTable.h"
#include "../FormulaWrapper.h"

using namespace AFormulaTable;

FormulaSetHashed::~FormulaSetHashed()
{
	this->Clear();
}

template <typename T>
void FormulaSetHashed::AddFormula(T formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == nullptr)
	{
		if(m_maxLength < formula->Length())
			m_maxLength = formula->Length();

		m_formulaMap[hash] =
			(formula->IsAtomic() && !formula->IsWrapped() ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas[formula->Length()].push_back(m_formulaMap[hash]);
	}
}

void FormulaSetHashed::Add(IFormula * formula)
{
	AddFormula(formula);
}

void FormulaSetHashed::Add(spIFormula formula)
{
	AddFormula(formula);
}

void FormulaSetHashed::Add(IFormulaSet& fset)
{
	FormulaSetHashed& fsetHash = dynamic_cast<FormulaSetHashed&>(fset);
	
	if(&fsetHash != nullptr)
	{
		for(unsigned i = 1; i <= fsetHash.m_maxLength; i++)
		{
			auto formulas = fsetHash[i];

			for(auto it = formulas.begin(); it != formulas.end(); it++)
			{
				this->Add(*it);
			}
		}
	}
}

unsigned FormulaSetHashed::Size()
{
	return m_formulaMap.size();
}

bool FormulaSetHashed::Contains(long hash)
{
	return m_formulaMap[hash] != nullptr;
}

spIFormula FormulaSetHashed::Get(long hash)
{
	return m_formulaMap[hash];
}

void FormulaSetHashed::Clear()
{
	m_formulaMap.clear();
	m_formulas.clear();
}

string FormulaSetHashed::ToString()
{
	if(Size() == 0)
		return "Empty set";

	stringstream stream;

	unsigned end = Size();

	stream << "{ ";

	for(unsigned i = 1; i <= m_maxLength; i++)
	{
		auto flist = (*this)[i];
		for(auto it = flist.begin(); it != flist.end(); it++)
		{
			stream << (*it)->ToString();
			auto nextIt = it;
			stream << ((i < m_maxLength || ++nextIt != flist.end()) ? ", " : "");
		}
	}

	stream<<" }";

	return stream.str();
}

vector<spIFormula> FormulaSetHashed::operator[](unsigned pos)
{
	return m_formulas[pos];
}