#ifndef AxiomContainer_h
#define AxiomContainer_h

#include "../../HilbertIncludes.h"
#include "../Compound/Axiom.h"
#include <vector>

using std::vector;

class AxiomContainer
{
public:
	AxiomContainer();
	virtual ~AxiomContainer();

	virtual Axiom * GetAxiom(unsigned index);
	void Add(IFormula * axiom);
	int Size();

	virtual string ToString();

protected:
	vector<Axiom*> m_axioms;
};

#endif