#include "Algorithm0x03.h"

#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetVector.h"
#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace General;
using std::endl;
using std::function;

Algorithm0x03::Algorithm0x03()
	: AlgorithmBase()
{
}

Algorithm0x03::~Algorithm0x03()
{
}

void Algorithm0x03::Run()
{
	if(m_finished)
		return;
	
	DoDeduction();

	//Cast m_sigma to the proper FormulaSet, if it fails return
	FormulaSetVector * sigma = dynamic_cast<FormulaSetVector*>(m_sigma);
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

	srand((unsigned)time(0));

	unordered_map<int, bool> usedFormulas;

	Stat_StartSize(m_sigma->Size());

	while(sigma->Size() <= m_sigmaLimit && !m_target->Equals(m_last))
	{
		if(pow(sigma->Size(), 2) == usedFormulas.size())
		{
			m_finished = false;
			break;
		}

		unsigned rnd1 = unsigned((rand() % sigma->Size()));
		unsigned rnd2 = unsigned((rand() % sigma->Size()));

		unsigned pair = CPAIR(rnd1, rnd2);

		if(usedFormulas[pair] == true)
			continue;

		usedFormulas[pair] = true;

		IFormula * first = (*sigma)[rnd1].get();
		IFormula * second = (*sigma)[rnd2].get();

		if(first->Length() > m_maxLength || second->Length() > m_maxLength)
			continue;

		if(MPBothWays(first, second, m_sigma))
			return;
		
		if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		{
			DELETEFORMULA(m_last);
		}

		if(first->IsWrapped())
			m_last = dynamic_cast<FormulaWrapper*>(first);
		else
			m_last = new FormulaWrapper(first->Clone());
	}

	m_finished = m_last != nullptr && m_last->Equals(m_target);
}

void Algorithm0x03::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	DELETE(m_sigma);

	m_sigma = new FormulaSetVector();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();
}

FSetType Algorithm0x03::GetFSetType()
{
	return FSET_VEC;
}