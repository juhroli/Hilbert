#ifndef Algorithm0x02_h
#define Algorithm0x02_h

#include "../HilbertDefines.h"
#include "AlgorithmBase.h"

/*
*
*/
class Algorithm0x02 : public AlgorithmBase
{
public:
	Algorithm0x02();
	~Algorithm0x02();
	void Run();
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	FSetType GetFSetType();
};

#endif