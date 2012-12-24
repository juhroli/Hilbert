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

void Algorithm0x03::Start()
{
	if(m_target == nullptr || m_axioms == nullptr)
		return;

	Stat_StartTimer();

	this->Run();

	Stat_EndTimer();
	Stat_EndSize(m_sigma->Size());
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

	//Set m_firstEnd to the end after deduction
	m_firstEnd = sigma->Size() - 1;
	m_firstEnd = m_firstEnd < 0 ? 0 : m_firstEnd;

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

		/*
		*	Using the Cantor pairing function to create one integer from two.
		*	Which is a bijection:
		*	f : N x N -> N (N := non-negative integers)
		*	defined by
		*	f(x, y) := ( (x + y) * (x + y + 1) ) / 2 + y
		*
		*	With this we can use an unordered map to identify
		*	which formula pairs were used already then skip them.
		*/

		unsigned pair = ((rnd1 + rnd2) * (rnd1 + rnd2 + 1)) / 2 + rnd2;

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

	if(m_last != nullptr && m_last->Equals(m_target))
		m_finished = true;
}

void Algorithm0x03::SetTask(IFormulaSet * Sigma, IFormula * F)
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
string Algorithm0x03::GetResult()
{
	stringstream stream;

	if(!m_finished)
	{
		stream<<"No results available.";
		return stream.str();
	}

	if(m_taskString.empty())
	{
		FormulaSetVector * sigma = dynamic_cast<FormulaSetVector*>(m_sigma);

		if(sigma->Size() > 0 && m_firstEnd != sigma->Size() - 1)
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
	else
	{
		stream<<m_taskString;
	}

	stream<<ResultString();

	return stream.str();
}

bool Algorithm0x03::ReadFromFile(string file)
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

FSetType Algorithm0x03::GetFSetType()
{
	return FSET_VEC;
}