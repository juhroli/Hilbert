#ifndef Algorithm0x03_h
#define Algorithm0x03_h

#include "../HilbertDefines.h"
#include "AlgorithmBase.h"

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
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	string GetResult();
	FSetType GetFSetType();
};

#endif