#ifndef AlgorithmBase_h
#define AlgorithmBase_h

#include "../HilbertIncludes.h"
#include "Stat.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "../Formula/Compound/ImplicationFormula.h"

/*
*	Base class of the algorithms.
*/
class AlgorithmBase : public Stat
{
public:
	AlgorithmBase();
	virtual ~AlgorithmBase() {}
	virtual void Start() = 0; //starts the algorithm processing
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

	virtual void Run() = 0; //run the algorithm
	virtual string ResultString(); //generates the result and returns it
	virtual void DoDeduction(); //do the deduction of the task
	virtual void AddAxiomsToSigma(); //add the axioms to sigma for easier iteration
};

#endif