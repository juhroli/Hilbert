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

void AxiomContainer::AddAxiom(Axiom * axiom)
{
	if(axiom == nullptr)
		return;
	
	m_axioms.push_back(axiom);
}

int AxiomContainer::GetSize()
{
	return m_axioms.size();
}