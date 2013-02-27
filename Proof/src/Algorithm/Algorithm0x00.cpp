#include "Algorithm0x00.h"
#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetList.h"
#include <functional>

using namespace General;
using std::endl;
using std::function;

Algorithm0x00::Algorithm0x00()
	: AlgorithmBase()
{
}

Algorithm0x00::~Algorithm0x00()
{
}

void Algorithm0x00::Run()
{
	if(m_finished)
		return;

	DoDeduction();

	//Cast m_sigma to the proper FormulaSet, if it fails return
	FormulaSetList * sigma = dynamic_cast<FormulaSetList*>(m_sigma);
	if(sigma == nullptr)
	{
		return;
	}

	if(m_taskString.empty())
	{
		stringstream stream;

		if(sigma->Size() > 0)
		{	
			stream << "After applying deduction: " << endl;
			stream << sigma->ToString();
		}
		stream<<" |- "<<m_target->ToString()<<endl<<endl<<endl;

		m_taskString = stream.str();
	}

	AddAxiomsToSigma();
	
	list<spIFormula>::iterator it = sigma->Begin();
	m_last = new FormulaWrapper(it->get()->Clone());

	Stat_StartSize(m_sigma->Size());

	for(; sigma->Size() <= m_sigmaLimit && it != sigma->End() && !m_target->Equals(it->get()); it++)
	{
		IFormula * iter = it->get();

		if(iter->Length() > m_maxLength)
			continue;

		if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		{
			DELETEFORMULA(m_last);
		}

		if(iter->IsWrapped())
			m_last = dynamic_cast<FormulaWrapper*>(iter);
		else
			m_last = new FormulaWrapper(iter->Clone());

		//The 2nd loop's end
		list<spIFormula>::iterator end = it;

		end++;

		/*
		*	Iterate through the rest of sigma and try to cut both ways.
		*/
		for(auto itS = sigma->Begin(); itS != end; itS++)
		{
			IFormula * iterS = itS->get();

			if(iterS->Length() > m_maxLength)
				continue;

			if(MPBothWays(iter, iterS, m_sigma))
				return;
		}
	}

	if(m_last != nullptr && m_last->Equals(m_target))
		m_finished = true;
	else if(it != sigma->End() && m_target->Equals(it->get()))
	{
		IFormula * iter = it->get();

		if(iter->IsWrapped())
			m_last = dynamic_cast<FormulaWrapper*>(iter);
		else
		{
			if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
			{
				DELETEFORMULA(m_last);
			}
			m_last = new FormulaWrapper(iter->Clone());
		}

		m_finished = true;
	}
}

void Algorithm0x00::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		DELETEFORMULA(m_last);

	DELETE(m_sigma);
	DELETEFORMULA(m_target);

	m_sigma = new FormulaSetList();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();

	m_finished = false;
}

FSetType Algorithm0x00::GetFSetType()
{
	return FSET_LIST;
}