#include "Algorithm0x02.h"

#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetVector.h"
#include <functional>

#ifdef _MSC_VER
	#include <ppl.h>
	using concurrency::parallel_for;
#endif

using namespace General;
using std::endl;
using std::function;

Algorithm0x02::Algorithm0x02()
	: AlgorithmBase()
{
}

Algorithm0x02::~Algorithm0x02()
{
}

void Algorithm0x02::Run()
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

	unsigned i = 0;

	AddAxiomsToSigma();
	unsigned prevSize = sigma->Size();

	Stat_StartSize(m_sigma->Size());

	sigma->SortFormulas();

	unordered_map< unsigned, unordered_map<unsigned, bool> > usedFormulas;

	for(i = 0; i < sigma->Size() && sigma->Size() <= m_sigmaLimit && !m_target->Equals((*sigma)[i].get()); i++)
	{
		IFormula * iter = (*sigma)[i].get();

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

		//The 2nd loop's iterator
		unsigned j = 0;

		/*
		*	Iterate through the rest of sigma and try to cut both ways.
		*/
		while(j <= i)
		{
			IFormula * iterS = (*sigma)[j].get();

			if(iterS->Length() > m_maxLength || usedFormulas[iter->HashCode()][iterS->HashCode()])
			{
				j++;
				continue;
			}

			usedFormulas[iter->HashCode()][iterS->HashCode()] = true;

			if(MPBothWays(iter, iterS, m_sigma))
				return;

			if(prevSize < sigma->Size())
			{
				prevSize = sigma->Size();

				sigma->SortFormulas();

				/*
				*	Correct the position of 'i', the new formula
				*	is before the i. formula after sorting.
				*	With parallel_for look for the m_last formula
				*	and if it was found, set i to its position.
				*/
				if(!(*sigma)[i]->Equals(iter))
				{
#ifdef _MSC_VER
					task_group tg;
					task_group_status status = tg.run_and_wait( [&]
					{
						parallel_for(unsigned(0), i, [&](unsigned x)
						{
							if((*sigma)[x]->Equals(m_last))
							{
								i = x;
								tg.cancel();
							}
						});
					});
#else
					for(unsigned x = i + 1; x >= 0; x--)
					{
						if((*sigma)[x]->Equals(m_last))
						{
							i = x;
							break;
						}
					}
#endif
					iter = (*sigma)[i].get();

					j = 0;
					continue;
				}
			}
			j++;
		}
	}

	if(m_last != nullptr && m_last->Equals(m_target))
		m_finished = true;
	else if(m_target->Equals((*sigma)[i].get()))
	{
		IFormula * iter = (*sigma)[i].get();

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

void Algorithm0x02::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	DELETE(m_sigma);

	m_sigma = new FormulaSetVector();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();
}

FSetType Algorithm0x02::GetFSetType()
{
	return FSET_VEC;
}