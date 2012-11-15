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
}

Algorithm0::~Algorithm0()
{
	DELETEFORMULA(m_target);
	delete m_sigma;
	m_sigma = __nullptr;
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

	//Set m_firstEnd to the end after deduction
	m_firstEnd = sigma->End();
	--m_firstEnd;

	//Put the axioms into sigma
	for(int i = 1; i <= m_axioms->GetSize(); i++)
	{
		sigma->Add(m_axioms->GetAxiom(i));
	}
	
	list<spIFormula>::iterator it = sigma->Begin();
	m_last = new FormulaWrapper(it->get());

	while(sigma->Size() <= m_sigmaLimit && it != sigma->End() && !m_target->Equals(it->get()))
	{
		IFormula * iter = it->get();

		if(iter->Length() > m_maxLength)
		{
			it++;
			continue;
		}
		
		m_last = new FormulaWrapper(iter);

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

			m_last = new FormulaWrapper(iterS);

			if(MPBothWays(iter, iterS, sigma))
				return;

			itS++;
		}

		it++;
	}

	if(m_last->Equals(m_target))
		m_finished = true;
	else if(it == sigma->End())
	{
		m_finished = false;
	}

}

void Algorithm0::SetAxioms(AxiomContainer * container)
{
    m_axioms = container;
}

void Algorithm0::SetTask(IFormulaSet * Sigma, IFormula * F)
{
	if(m_sigma != __nullptr)
	{
		delete m_sigma;
		m_sigma = __nullptr;
	}
	m_sigma = new FormulaSetList();

	m_sigma->Add(*Sigma);
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