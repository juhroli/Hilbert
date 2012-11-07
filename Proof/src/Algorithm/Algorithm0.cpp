#include "Algorithm0.h"
#include "General.h"
#include "../Formula/Containers/Sets/FormulaSetList.h"
#include "../Formula/Containers/Sets/FormulaSetVector.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Containers/AFormulaTable.h"

using namespace General;
using namespace AFormulaTable;
using std::endl;

Algorithm0::Algorithm0()
	: IAlgorithm()
	, m_sigmaLimit(1000)
{
	if(m_sigma == __nullptr)
		m_sigma = new FormulaSetList();
}

Algorithm0::~Algorithm0()
{

}

void Algorithm0::Start()
{
	if(m_target == __nullptr || m_axioms == __nullptr)
		return;
	this->Run();
}

void Algorithm0::Run()
{
	//Do the deduction
	IFormula * deduct = m_target;
	IFormula * res = __nullptr;
	while(Deduction(deduct, m_sigma, res))
	{
		m_target = res;
		deduct = m_target;
		res = __nullptr;
	}
	res = __nullptr;
	deduct = __nullptr;

	//Cast m_sigma to the proper FormulaSet, if it fails return
	FormulaSetList * sigma = dynamic_cast<FormulaSetList*>(m_sigma);
	if(sigma == __nullptr)
	{
		return;
	}
	
	list<spIFormula>::iterator it = sigma->Begin();
	list<spIFormula>::iterator end = sigma->End();
	if(sigma->Size() > 0 && end != sigma->Begin())
	{
		--end;
		m_firstEnd = end;
	}

	//Iterate from sigma's beginning in the second loop?
	bool begin = false;

	m_last = new FormulaWrapper(it->get());

	while(sigma->Size() > 0 && sigma->Size() <= m_sigmaLimit && it != sigma->End() && (!m_last->Equals(m_target)
		|| (!m_last->IsTemp() && !m_last->IsAtomic())))
	{
		if(it->get()->Length() > m_maxLength)
		{
			it++;
			continue;
		}

		IFormula * iter = it->get();
		m_last = new FormulaWrapper(iter);

		/*
		*	Iterate through the axioms and try to cut both ways.
		*/
		for(int i = 1; i <= m_axioms->GetSize(); i++)
		{
			if(MPBothWays(iter, m_axioms->GetAxiom(i)))
				return;
		}

		//The 2nd loop's iterator
		list<spIFormula>::iterator itS;

		/*
		*	Check if the iterator should be pointed to the beginning.
		*	After we finish with sigma's original content the
		*	itS iterator should start from the beginning of sigma
		*	because the it iterator points to a formula created by
		*	modus ponens.
		*/
		if(begin)
			itS = sigma->Begin();
		else
		{
			itS = it;
			if(itS != end && (++itS) == sigma->End())
				continue;
			if(itS != sigma->Begin())
				--itS;
		}

		/*
		*	Iterate through the rest of sigma and try to cut both ways.
		*/
		do
		{
			if(++itS == it || itS == sigma->End())
			{
				continue;
			}

			if(itS->get()->Length() > m_maxLength)
				continue;

			IFormula * iterS = itS->get();

			m_last = new FormulaWrapper(iterS);

			if(MPBothWays(iter, iterS))
				return;
			
		} while(itS != end && itS != sigma->End() && (!m_last->Equals(m_target)
			|| (!m_last->IsTemp() && !m_last->IsAtomic())));

		auto endpp = end;
		if(it == end && ++endpp != sigma->End())
		{
			end = it;
			end++;
			begin = true;
		}

		it++;
	}

	if(m_last->Equals(m_target))
		m_finished = true;

	if(!m_finished)
	{
		if(it == sigma->End() || it == end)
		{
			m_finished = false;
		}
	}
}

void Algorithm0::SetAxioms(AxiomContainer * container)
{
    m_axioms = container;
}

void Algorithm0::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	m_sigma = Sigma;
	m_target = F->Clone();
}

bool Algorithm0::IsFinished()
{
	return m_finished;
}

/*
*	Writes the result to a string showing the steps of the proof.
*/
string Algorithm0::GetResult()
{
	stringstream stream;

	if(!m_finished)
	{
		stream<<"No results available.";
		return stream.str();
	}

	FormulaSetList * sigma = dynamic_cast<FormulaSetList*>(m_sigma);

	if(sigma->Size() > 0 && m_firstEnd != sigma->End())
	{
		list<spIFormula>::iterator it = sigma->Begin();
		list<spIFormula>::iterator end = ++m_firstEnd;
		m_firstEnd--;
	
		stream << "After applying deduction: " << endl << "{ ";

		do {
			stream << (it++)->get()->ToString() << ((it != m_firstEnd) ? ", " : "");
		} while(it != end);
		stream<<" } ";
	}
	stream<<"|- "<<m_target->ToString()<<endl<<endl<<endl;

	stream<<"The proof is:"<<endl;
	FormulaSetList fset;

	std::function<void (IFormula*)> add =
		[&] (IFormula * a)
		{
			FormulaWrapper * F = dynamic_cast<FormulaWrapper*>(a);
			if(F != nullptr)
			{
				add(F->GetOrigin().first);
				add(F->GetOrigin().second);
				fset.Add(sigma->Get(F->HashCode()));
			}
	
		};

	//Add the formulas into a FormulaSet -> duplications can be avoided
	add(m_last);
	
	int i = 1;

	//Write the steps into the stream
	for(auto it = fset.Begin(); it != fset.End(); it++, i++)
	{
		FormulaWrapper * F = dynamic_cast<FormulaWrapper*>(it->get());
		stream<<i<<". ";
		if(F == __nullptr)
			stream<<it->get()->ToString()<<"    (in Sigma)"<<endl<<endl;
		else if(F->IsFromSigma())
			stream<<F->ToString()<<"    "<<F->GetReplaces()<<endl<<endl;
		else if(!F->Equals(m_last) || !F->IsTemp())
			stream<<F->ToString()<<"    : Cut "<<F->GetOrigin().second->ToString()<<" with "<<F->GetOrigin().first->ToString()<<" "<<F->GetReplaces()<<endl<<endl;
		else
			stream<<m_target->ToString()<<"    : Cut "<<F->GetOrigin().second->ToString()<<" with "<<F->GetOrigin().first->ToString()<<" "<<F->GetReplaces()<<"["<<F->ToString()<<"/"<<m_target->ToString()<<"]"<<endl<<endl;
	}

	return stream.str();
}

void Algorithm0::SetSigmaLimit(unsigned limit)
{
	m_sigmaLimit = limit;
}