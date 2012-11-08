#pragma once

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "../Formula/Compound/ImplicationFormula.h"

/*
*	Base class of the algorithms.
*/
class IAlgorithm
{
public:
	IAlgorithm()
		: m_axioms(__nullptr)
		, m_mpResults(__nullptr)
		, m_target(__nullptr)
		, m_sigma(__nullptr)
		, m_result("")
		, m_finished(false)
		, m_maxLength(20)
	{
	}
	virtual ~IAlgorithm() {}
	virtual void Start() = 0;
	virtual void Run() = 0;
	virtual void SetAxioms(AxiomContainer * container) = 0;
	virtual void SetTask(IFormulaSet * Sigma, IFormula * F) = 0;
	virtual bool IsFinished() = 0;
	virtual string GetResult() = 0;

	void SetMaxLength(unsigned length);
	bool MPBothWays(IFormula * a, IFormula * b, IFormulaSet * fset);
	
protected:
    AxiomContainer * m_axioms; //axioms to use for the algorithm
    IFormulaSet * m_sigma; //the set of formulas
    IFormula * m_target; //the target formula
    IFormulaSet * m_mpResults; //modus ponens results in formulawrapper
	string m_result; //the proof
	bool m_finished; //is the algorithm finished?
	FormulaWrapper * m_last; //the last formula in the iteration
	unsigned m_maxLength; //maximum length of the formulas
};