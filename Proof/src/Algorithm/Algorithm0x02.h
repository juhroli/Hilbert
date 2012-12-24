#ifndef Algorithm0x02_h
#define Algorithm0x02_h

#include "../HilbertIncludes.h"
#include "AlgorithmBase.h"

class FormulaWrapper;

/*
*
*/
class Algorithm0x02 : public AlgorithmBase
{
public:
	Algorithm0x02();
	~Algorithm0x02();
	void Start();
	void Run();
	void SetTask(IFormulaSet * Sigma, IFormula * F);
	string GetResult();
	bool ReadFromFile(string file);
	FSetType GetFSetType();

private:
	unsigned m_firstEnd; //position to sigma's end after deduction
};

#endif