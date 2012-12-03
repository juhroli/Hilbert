#ifndef Algorithm0x03_h
#define Algorithm0x03_h

#include "../HilbertIncludes.h"
#include "AlgorithmBase.h"

class FormulaWrapper;

/*
*
*/
class Algorithm0x03 : public AlgorithmBase
{
public:
	Algorithm0x03();
	~Algorithm0x03();
	void Start();
	void Run();
	void SetAxioms(AxiomContainer * container);
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	bool IsFinished();
	string GetResult();
	void SetSigmaLimit(unsigned limit);

private:
	unsigned m_firstEnd; //position to sigma's end after deduction
	unsigned m_sigmaLimit; //limit sigma's size
};

#endif