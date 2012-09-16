#pragma once

#include "Axiom.h"
#include <vector>

using std::vector;

class AxiomContainer
{
public:
	AxiomContainer();
	virtual ~AxiomContainer();

	virtual Axiom * GetAxiom(unsigned index);
	void AddAxiom(Axiom * axiom);
	int GetSize();

protected:
	vector<Axiom*> m_axioms;
};