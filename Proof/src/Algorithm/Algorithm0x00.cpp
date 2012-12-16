#include "Algorithm0x00.h"
#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetList.h"
#include <functional>

using namespace General;
using std::endl;
using std::function;

Algorithm0x00::Algorithm0x00()
	: AlgorithmBase()
	, m_sigmaLimit(250)
{
}

Algorithm0x00::~Algorithm0x00()
{
	if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		DELETEFORMULA(m_last);

	DELETEFORMULA(m_target);

	if(m_sigma != nullptr)
	{
		delete m_sigma;
		m_sigma = nullptr;
	}
}

void Algorithm0x00::Start()
{
	if(m_target == nullptr || m_axioms == nullptr)
		return;

	this->Run();

	Stat_EndTimer();
	Stat_EndSize(m_sigma->Size());
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

	//Set m_firstEnd to the end after deduction
	m_firstEnd = sigma->End();
	if(sigma->Size() > 0)
		--m_firstEnd;

	AddAxiomsToSigma();
	
	list<spIFormula>::iterator it = sigma->Begin();
	m_last = new FormulaWrapper(it->get()->Clone());

	//Start the Stat timer
	Stat_StartTimer();
	Stat_StartSize(m_sigma->Size());

	while(sigma->Size() <= m_sigmaLimit && it != sigma->End() && !m_target->Equals(it->get()))
	{
		IFormula * iter = it->get();

		if(iter->Length() > m_maxLength)
		{
			it++;
			continue;
		}

		if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		{
			DELETEFORMULA(m_last);
		}

		if(iter->IsWrapped())
			m_last = dynamic_cast<FormulaWrapper*>(iter);
		else
			m_last = new FormulaWrapper(iter->Clone());

		//The 2nd loop's iterator
		list<spIFormula>::iterator itS = sigma->Begin();
		list<spIFormula>::iterator end = it;

		end++;

		/*
		*	Iterate through the rest of sigma and try to cut both ways.
		*/
		while(itS != end)
		{
			IFormula * iterS = itS->get();

			if(iterS->Length() > m_maxLength)
				continue;

			if(MPBothWays(iter, iterS, m_sigma))
				return;

			itS++;
		}

		it++;
	}

	if(m_last != nullptr && m_last->Equals(m_target))
		m_finished = true;
	else if(it == sigma->End())
	{
		m_finished = false;
	}
}

void Algorithm0x00::SetAxioms(AxiomContainer * container)
{
    m_axioms = container;
}

void Algorithm0x00::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_sigma != nullptr)
	{
		delete m_sigma;
		m_sigma = nullptr;
	}
	m_sigma = new FormulaSetList();

	m_sigma->Add(*Sigma);
	m_target = F->Clone();
}

bool Algorithm0x00::IsFinished()
{
	return m_finished;
}

/*
*	Writes the result to a string showing the steps of the proof.
*/
string Algorithm0x00::GetResult()
{
	stringstream stream;

	if(!m_finished)
	{
		stream<<"No results available.";
		return stream.str();
	}

	if(m_taskString.empty())
	{
		FormulaSetList * sigma = dynamic_cast<FormulaSetList*>(m_sigma);

		if(sigma->Size() > 0 && m_firstEnd != sigma->End())
		{
			list<spIFormula>::iterator it = sigma->Begin();
			list<spIFormula>::iterator end = m_firstEnd;
			end++;
	
			stream << "After applying deduction: " << endl << "{ ";

			do {
				stream << (it++)->get()->ToString();
				stream << ((it != end) ? ", " : "");
			} while(it != end);
			stream<<" } ";
		}
		stream<<"|- "<<m_target->ToString()<<endl<<endl<<endl;
		
		m_taskString = stream.str();
	}
	else
	{
		stream<<m_taskString;
	}

	stream<<ResultString();

	return stream.str();
}

void Algorithm0x00::SetSigmaLimit(unsigned limit)
{
	m_sigmaLimit = limit;
}