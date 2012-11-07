#include "HilbertAxioms.h"
#include "../../Input/FormulaParser.h"

using namespace FormulaParser;

/*
*	1. (F -> (G -> H)) -> ((F -> G) -> (F -> H))
*	2. F -> (G -> F)
*	3. ((F -> ~) -> ~) -> F
*	Where ~ is false
*/
HilbertAxioms::HilbertAxioms()
{
	m_axioms.push_back(static_cast<Axiom*>( ParseTemp("(a -> (b -> c)) -> ((a -> b) -> (a -> c))") ));
	m_axioms.push_back(static_cast<Axiom*>( ParseTemp("a -> (b -> a)") ));
	stringstream stream;
	stream<<"((a -> "<<FALSE<<") -> "<<FALSE<<") -> a\0";
	m_axioms.push_back(static_cast<Axiom*>( ParseTemp(stream.str()) ));
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
	return __nullptr;
}