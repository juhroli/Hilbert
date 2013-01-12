#include "AxiomContainer.h"

AxiomContainer::AxiomContainer()
{
}

AxiomContainer::~AxiomContainer()
{
	this->m_axioms.clear();
}

Axiom * AxiomContainer::GetAxiom(unsigned index)
{
	if( index >= 1 || index <= m_axioms.size() )
	{
		return new Axiom( *m_axioms[index-1] );
	}
	return nullptr;
}

void AxiomContainer::Add(IFormula * axiom)
{
	Axiom * input = nullptr;
	if(axiom == nullptr
		|| (input = dynamic_cast<Axiom*>(axiom)) == nullptr)
		return;
	
	m_axioms.push_back(input);
}

unsigned AxiomContainer::Size()
{
	return m_axioms.size();
}

string AxiomContainer::ToString()
{
	if(Size() == 0)
	return "Empty set";

	stringstream stream;

	unsigned end = Size();

	stream << "{ ";

	for(unsigned i = 0; i < end; i++)
	{
		stream << m_axioms[i]->ToString();
		stream << ((i != end) ? ", " : "");
	}

	stream<<" }";

	return stream.str();
}