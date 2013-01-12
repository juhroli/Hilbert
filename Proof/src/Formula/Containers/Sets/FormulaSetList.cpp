#include "FormulaSetList.h"
#include "../AFormulaTable.h"
#include "../FormulaWrapper.h"

using namespace AFormulaTable;

FormulaSetList::~FormulaSetList()
{
	this->Clear();
}

template<typename T>
void FormulaSetList::AddFormula(T formula)
{
	long hash = formula->HashCode();

	if(m_formulaMap[hash] == nullptr)
	{
		m_formulaMap[hash] =
			(formula->IsAtomic() && !formula->IsWrapped() ? spIFormula(GetAtomicFormula(formula->HashCode())) : spIFormula(formula));
		m_formulas.push_back(m_formulaMap[hash]);
	}
}

void FormulaSetList::Add(IFormula * formula)
{
	AddFormula(formula);
}

void FormulaSetList::Add(spIFormula formula)
{
	AddFormula(formula);
}

void FormulaSetList::Add(IFormulaSet& fset)
{
	FormulaSetList& fsetList = dynamic_cast<FormulaSetList&>(fset);
	
	if(&fsetList != nullptr)
	{
		list<spIFormula>::iterator end = fsetList.End();
		for(auto it = fsetList.Begin(); it != end; it++)
		{
			this->Add(*it);
		}
	}
}

list<spIFormula>::iterator FormulaSetList::Begin()
{
	return m_formulas.begin();
}

list<spIFormula>::iterator FormulaSetList::End()
{
	return m_formulas.end();
}

unsigned FormulaSetList::Size()
{
	return m_formulas.size();
}

bool FormulaSetList::Contains(long hash)
{
	return m_formulaMap[hash] != nullptr;
}

spIFormula FormulaSetList::Get(long hash)
{
	return m_formulaMap[hash];
}

void FormulaSetList::Clear()
{
	m_formulaMap.clear();
	m_formulas.clear();
}

string FormulaSetList::ToString()
{
	if(Size() == 0)
		return "Empty set";

	stringstream stream;

	list<spIFormula>::iterator it = Begin();
	list<spIFormula>::iterator end = End();
	
	stream << "{ ";

	do {
		stream << (it++)->get()->ToString();
		stream << ((it != end) ? ", " : "");
	} while(it != end);

	stream<<" }";

	return stream.str();
}