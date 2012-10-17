#pragma once

#include "AxiomContainer.h"

class HilbertAxioms : public AxiomContainer
{
public:
	HilbertAxioms();
	~HilbertAxioms() {}
	Axiom * GetAxiom(unsigned index);
};