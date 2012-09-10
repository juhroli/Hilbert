#include "HilbertAxioms.h"

/**
*	1. (F -> (G -> H)) -> ((F -> G) -> (F -> H))
*	2. F -> (G -> F)
*	3. ((F -> f) -> f) -> F
*	Where f is false
*/
HilbertAxioms::HilbertAxioms()
{
	for(int i=0; i<3; i++)
	{
		m_axioms[i] = new Axiom();
	}
}

/**
*	index 1, 2, 3
*/
Axiom * HilbertAxioms::GetAxiom(int index)
{
	if(index >= 1 || index <= 3)
	{
		return m_axioms[index-1];
	}
	return 0;
}