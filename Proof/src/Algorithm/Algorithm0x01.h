#ifndef Algorithm0x01_h
#define Algorithm0x01_h

#include "../HilbertDefines.h"
#include "AlgorithmBase.h"

/*
*
*/
class Algorithm0x01 : public AlgorithmBase
{
public:
	Algorithm0x01();
	~Algorithm0x01();
	void Start();
	void Run();
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	string GetResult();
	bool ReadFromFile(string file);
	FSetType GetFSetType();
};

#endif