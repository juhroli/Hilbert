#ifndef Algorithm0x00_h
#define Algorithm0x00_h

#include "../HilbertIncludes.h"
#include "AlgorithmBase.h"

class FormulaWrapper;

/*
*
*/
class Algorithm0x00 : public AlgorithmBase
{
public:
	Algorithm0x00();
	~Algorithm0x00();
	void Start();
	void Run();
	void SetAxioms(AxiomContainer * container);
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	bool IsFinished();
	string GetResult();
	void SetSigmaLimit(unsigned limit);

private:
	list<spIFormula>::iterator m_firstEnd; //iterator to sigma's end after deduction
	unsigned m_sigmaLimit; //limit sigma's size
};

#endif