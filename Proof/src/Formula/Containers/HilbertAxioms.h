#ifndef HilbertAxioms_h
#define HilbertAxioms_h

#include "../../HilbertIncludes.h"
#include "AxiomContainer.h"

class HilbertAxioms : public AxiomContainer
{
public:
	HilbertAxioms();
	~HilbertAxioms();
	Axiom * GetAxiom(unsigned index);
};

#endif