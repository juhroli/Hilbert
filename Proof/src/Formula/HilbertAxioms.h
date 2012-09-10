#pragma once

#include "Axiom.h"

class HilbertAxioms
{
public:
	HilbertAxioms();
	Axiom * GetAxiom(int index);

private:
	//vector<Axiom> m_axioms;
	Axiom * m_axioms[3];
};