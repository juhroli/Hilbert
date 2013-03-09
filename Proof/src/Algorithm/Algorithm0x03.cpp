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
	, m_seed(unsigned(time(0)))
{
}

Algorithm0x03::Algorithm0x03(unsigned seed)
	: AlgorithmBase()
	, m_seed(seed)
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
			stream << sigma->ToString();
		}
		stream<<" |- "<<m_target->ToString();

		m_taskString = stream.str();
	}

	AddAxiomsToSigma();

	srand(m_seed);

	unordered_map<unsigned, bool> usedFormulas;

	Stat_StartSize(m_sigma->Size());

	while(sigma->Size() <= m_sigmaLimit)
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

		if(m_finished = (first->Equals(m_target) || second->Equals(m_target)))
		{
			IFormula * target = nullptr;

			if(first->Equals(m_target))
				target = first;
			else
				target = second;

			if(target->IsWrapped())
				m_last = dynamic_cast<FormulaWrapper*>(target);
			else
				m_last = new FormulaWrapper(target->Clone());

			return;
		}

		if(first->Length() > m_maxLength || second->Length() > m_maxLength)
			continue;

		if(MPBothWays(first, second, m_sigma))
			return;

	}

	m_finished = m_last != nullptr && m_last->Equals(m_target);
}

void Algorithm0x03::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		DELETEFORMULA(m_last);

	DELETE(m_sigma);
	DELETEFORMULA(m_target);

	m_sigma = new FormulaSetVector();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();

	m_finished = false;
}

FSetType Algorithm0x03::GetFSetType()
{
	return FSET_VEC;
}

string Algorithm0x03::GetResult()
{
	stringstream stream;
	
	stream << AlgorithmBase::GetResult() << endl << endl;

	stream << "Used random seed: " << m_seed;

	return stream.str();
}