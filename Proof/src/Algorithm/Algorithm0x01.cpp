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

void Algorithm0x01::Start()
{
	if(m_target == nullptr || m_axioms == nullptr)
		return;

	Stat_StartTimer();

	this->Run();

	Stat_EndTimer();
	Stat_EndSize(m_sigma->Size());
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
	
	unsigned prevSize = sigma->Size();

	Stat_StartSize(m_sigma->Size());

	unsigned depth = 0;

	unordered_map<unsigned, bool> usedFormulas;

	unsigned i = 1;

	while(i <= m_maxLength && sigma->Size() < m_sigmaLimit)
	{
		auto actList = (*sigma)[i];
		unsigned j = 1;

		while(j <= i)
		{
			auto actListS = (*sigma)[j];
			unsigned it = 0;

			while(it < actList.size())
			{
				IFormula * iter = actList[it].get();

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

				if(m_target->Equals(iter))
				{
					m_finished = true;
					return;
				}

				unsigned itS = 0;

				if(i == j)
					itS = it;

				while(itS < actListS.size())
				{
					IFormula * iterS = actListS[itS].get();

					unsigned pair = CPAIR( CPAIR(i, j), CPAIR(it, itS) );

					if(iterS->Length() > m_maxLength || usedFormulas[pair])
					{
						itS++;
						continue;
					}

					usedFormulas[pair] = true;

					if(MPBothWays(iter, iterS, m_sigma))
						return;

					if(prevSize < sigma->Size())
					{
						prevSize = sigma->Size();

						if(m_last->Length() <= i)
						{
							i = m_last->Length() - 1;
							j = actListS.size();
							it = actList.size();
							itS = actListS.size();
							continue;
						}
					}
					itS++;
				}
				it++;
			}
			j++;
		}
		i++;
	}

	if(m_last != nullptr && m_last->Equals(m_target))
		m_finished = true;
}

void Algorithm0x01::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	DELETE(m_sigma);

	m_sigma = new FormulaSetHashed();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();
}

/*
*	Writes the result to a string showing the steps of the proof.
*/
string Algorithm0x01::GetResult()
{
	stringstream stream;

	if(!m_finished)
	{
		stream<<"No results available.";
		return stream.str();
	}
	
	stream<<m_taskString;

	stream<<ResultString();

	return stream.str();
}

bool Algorithm0x01::ReadFromFile(string file)
{
	m_reader = new FileReader(file, FSET_HASHED);

	if( m_reader->ReadFile() )
	{
		this->SetTask(m_reader->GetSet(), m_reader->GetTarget());
		this->SetAxioms(m_reader->GetAxioms());
	}
	else
	{
		return false;
	}

	return true;
}

FSetType Algorithm0x01::GetFSetType()
{
	return FSET_HASHED;
}