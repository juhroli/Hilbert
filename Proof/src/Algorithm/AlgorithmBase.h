#ifndef AlgorithmBase_h
#define AlgorithmBase_h

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "../Formula/Compound/ImplicationFormula.h"

/*
*	Base class of the algorithms.
*/
class AlgorithmBase
{
public:
	AlgorithmBase()
		: m_axioms(nullptr)
		, m_mpResults(nullptr)
		, m_target(nullptr)
		, m_sigma(nullptr)
		, m_last(nullptr)
		, m_result("")
		, m_finished(false)
		, m_maxLength(20)
	{
	}
	virtual ~AlgorithmBase() {}
	virtual void Start() = 0;
	virtual void Run() = 0;
	virtual void SetAxioms(AxiomContainer * container) = 0;
	virtual void SetTask(IFormulaSet * Sigma, IFormula * F) = 0;
	virtual bool IsFinished() = 0;
	virtual string GetResult() = 0;

	void SetMaxLength(unsigned length);
	bool MPBothWays(IFormula * a, IFormula * b, IFormulaSet*& fset);
	
protected:
    AxiomContainer * m_axioms; //axioms to use for the algorithm
    IFormulaSet * m_sigma; //the set of formulas
    IFormula * m_target; //the target formula
    IFormulaSet * m_mpResults; //modus ponens results in formulawrapper
	string m_result; //the proof
	bool m_finished; //is the algorithm finished?
	FormulaWrapper * m_last; //the last formula in the iteration
	unsigned m_maxLength; //maximum length of the formulas
	string m_resString; //this string contains the proof
	string m_taskString; //this is the task's string after deduction -> Sigma |- F

	virtual string ResultString(); //generates the result and returns it
	virtual void DoDeduction(); //do the deduction of the task
	virtual void AddAxiomsToSigma(); //add the axioms to sigma for easier iteration
};

#endif