#ifndef AxiomContainer_h
#define AxiomContainer_h

#include "../../HilbertDefines.h"
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
	unsigned Size();

	virtual string ToString();

protected:
	vector<Axiom*> m_axioms;
};

#endif