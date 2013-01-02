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
	if(m_last != nullptr && (m_last->IsFromSigma() || m_last->IsAxiom()))
		DELETEFORMULA(m_last);

	DELETEFORMULA(m_target);

	if(m_sigma != nullptr)
	{
		delete m_sigma;
		m_sigma = nullptr;
	}

	if(m_reader != nullptr)
	{
		delete m_reader;
		m_reader = nullptr;
	}
}

void Algorithm0x02::Start()
{
	if(m_target == nullptr || m_axioms == nullptr)
		return;

	Stat_StartTimer();

	this->Run();

	Stat_EndTimer();
	Stat_EndSize(m_sigma->Size());
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

	//Set m_firstEnd to the end after deduction
	m_firstEnd = sigma->Size() - 1;
	m_firstEnd = m_firstEnd < 0 ? 0 : m_firstEnd;

	if(m_taskString.empty())
	{
		stringstream stream;

		if(sigma->Size() > 0)
		{	
			stream << "After applying deduction: " << endl << "{ ";

			for(unsigned i = 0; i <= m_firstEnd; i++)
			{
				stream << (*sigma)[i].get()->ToString();
				stream << ((i != m_firstEnd) ? ", " : "");
			}
			stream<<" } ";
		}
		stream<<"|- "<<m_target->ToString()<<endl<<endl<<endl;

		m_taskString = stream.str();
	}

	unsigned i = 0;

	AddAxiomsToSigma();
	unsigned prevSize = sigma->Size();

	Stat_StartSize(m_sigma->Size());

	sigma->SortFormulas();

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
		unsigned end = i + 1;

		/*
		*	Iterate through the rest of sigma and try to cut both ways.
		*/
		while(j < end)
		{
			IFormula * iterS = (*sigma)[j].get();

			if(iterS->Length() > m_maxLength)
			{
				j++;
				continue;
			}

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
					for(unsigned x = 0; x < i; x++)
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

				/*
				*	The same applies here as above.
				*/
				if(!(*sigma)[j]->Equals(iterS))
				{
#ifdef _MSC_VER
					task_group tg;
					task_group_status status = tg.run_and_wait( [&]
					{
						parallel_for(unsigned(i + 1), j, [&](unsigned x)
						{
							if((*sigma)[x]->Equals(m_last))
							{
								j = x;
								tg.cancel();
							}
						});
					});
#else
					for(unsigned x = i + 1; x < j; x++)
					{
						if((*sigma)[x]->Equals(m_last))
						{
							j = x;
							break;
						}
					}
#endif
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
			m_last = new FormulaWrapper(iter->Clone());

		m_finished = true;
	}
}

void Algorithm0x02::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_sigma != nullptr)
	{
		delete m_sigma;
		m_sigma = nullptr;
	}
	m_sigma = new FormulaSetVector();

	if(Sigma != nullptr)
		m_sigma->Add(*Sigma);

	m_target = F->Clone();
}

/*
*	Writes the result to a string showing the steps of the proof.
*/
string Algorithm0x02::GetResult()
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

bool Algorithm0x02::ReadFromFile(string file)
{
	m_reader = new FileReader(file, FSET_VEC);

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

FSetType Algorithm0x02::GetFSetType()
{
	return FSET_VEC;
}