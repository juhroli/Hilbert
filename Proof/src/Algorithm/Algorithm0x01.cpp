#include "Algorithm0x01.h"

#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetHashed.h"
#include <functional>

using namespace General;
using std::endl;
using std::function;

Algorithm0x01::Algorithm0x01()
	: AlgorithmBase()
{
}

Algorithm0x01::~Algorithm0x01()
{
}

void Algorithm0x01::Run()
{
	if(m_finished)
		return;

	DoDeduction();

	//Cast m_sigma to the proper FormulaSet, if it fails return
	FormulaSetHashed * sigma = dynamic_cast<FormulaSetHashed*>(m_sigma);
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

		stream << "|- " << m_target->ToString() <<endl<<endl<<endl;
		
		m_taskString = stream.str();
	}

	AddAxiomsToSigma();
	
	// previous size of sigma
	unsigned prevSize = sigma->Size();

	Stat_StartSize(m_sigma->Size());

	unordered_map<unsigned, bool> usedFormulas;

	for(unsigned i = 1; i <= m_maxLength && sigma->Size() < m_sigmaLimit; i++)
	{
		auto actList = (*sigma)[i];

		for(unsigned j = 1; j <= i; j++)
		{
			auto actListS = (*sigma)[j];

			for(unsigned it = 0; it < actList.size(); it++)
			{
				IFormula * iter = actList[it].get();

				if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
				{
					DELETEFORMULA(m_last);
				}

				if(iter->IsWrapped())
					m_last = dynamic_cast<FormulaWrapper*>(iter);
				else
					m_last = new FormulaWrapper(iter->Clone());

				if(m_finished = m_target->Equals(iter))
					return;

				for(unsigned itS = i == j ? it : 0; itS < actListS.size(); itS++)
				{
					IFormula * iterS = actListS[itS].get();

					unsigned pair = CPAIR( CPAIR(i, j), CPAIR(it, itS) );

					if(usedFormulas[pair])
						continue;

					usedFormulas[pair] = true;

					if(MPBothWays(iter, iterS, m_sigma))
						return;

					if(prevSize < sigma->Size())
					{
						prevSize = sigma->Size();

						if(m_last->Length() <= i)
						{
							i = m_last->Length() - 1;
							j = (itS = actListS.size());
							it = actList.size();
							continue;
						}
					}
				}
			}
		}
	}

	m_finished = m_last != nullptr && m_last->Equals(m_target);
}

void Algorithm0x01::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		DELETEFORMULA(m_last);

	DELETE(m_sigma);
	DELETEFORMULA(m_target);

	m_sigma = new FormulaSetHashed();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();

	m_finished = false;
}

FSetType Algorithm0x01::GetFSetType()
{
	return FSET_HASHED;
}