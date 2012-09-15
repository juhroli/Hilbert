#include "HilbertAxioms.h"
#include "../Input/InputHandler.h"

using namespace InputHandler;

/*
*	1. (F -> (G -> H)) -> ((F -> G) -> (F -> H))
*	2. F -> (G -> F)
*	3. ((F -> ~) -> ~) -> F
*	Where ~ is false
*/
HilbertAxioms::HilbertAxioms()
{
	m_axioms.push_back(static_cast<Axiom*>( StringToFormula("(F -> (G -> H)) -> ((F -> G) -> (F -> H))", F_AXIOM) ));
	m_axioms.push_back(static_cast<Axiom*>( StringToFormula("F -> (G -> F)", F_AXIOM) ));
	stringstream stream;
	stream<<"((F -> "<<FALSE<<") -> "<<FALSE<<") -> F\0";
	m_axioms.push_back(static_cast<Axiom*>( StringToFormula(stream.str(), F_AXIOM) ));
}

HilbertAxioms::~HilbertAxioms()
{
	m_axioms.clear();
}

/*
*	Indexed from 1.
*/
Axiom * HilbertAxioms::GetAxiom(unsigned index)
{
	if(index >= 1 || index <= 3)
	{
		return new Axiom( *m_axioms[index-1] );
	}
	return NULL;
}