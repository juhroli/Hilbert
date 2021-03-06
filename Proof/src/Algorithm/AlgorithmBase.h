#ifndef AlgorithmBase_h
#define AlgorithmBase_h

#include "../HilbertDefines.h"
#include "Stat.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/Sets/IFormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"
#include "../Formula/Containers/FormulaWrapper.h"
#include "../Formula/Compound/ImplicationFormula.h"
#include "../Input/FileReader.h"

/*
*	Base class of the algorithms.
*/
class AlgorithmBase : public Stat
{
public:
	AlgorithmBase();
	virtual ~AlgorithmBase();
	
	virtual void SetTask(IFormulaSet * Sigma, IFormula * F) = 0; //sets the formula set and the target formula
	virtual FSetType GetFSetType() = 0; //returns the FormulaSet type of the member FormulaSet
	virtual void SetAxioms(AxiomContainer * container);
	virtual bool IsFinished();
	virtual string GetResult(); //returns the result string

	void Start(); //starts the algorithm processing
	bool ReadFromFile(string file); //reads the task from a file
	void SetMaxLength(unsigned length); //sets the maximum length of the formulas
	void SetSigmaLimit(unsigned limit); //sets sigma's maximum size
	void SetLatex(bool use);
	
protected:
    AxiomContainer * m_axioms; //axioms to use for the algorithm
    IFormulaSet * m_sigma; //the set of formulas
    IFormula * m_target; //the target formula
	bool m_finished; //is the algorithm finished?
	FormulaWrapper * m_last; //the last formula in the iteration
	unsigned m_maxLength; //maximum length of the formulas
	unsigned m_sigmaLimit; //limit sigma's size
	string m_resString; //this string contains the proof
	string m_taskString; //this is the task's string after deduction -> Sigma |- F
	FileReader * m_reader; //for reading formulas and axioms from a file directly into the members
	bool m_useLatex; //true if the output should be in latex source format

	bool MPBothWays(IFormula * a, IFormula * b, IFormulaSet*& fset); //does modus ponens

	virtual void Run() = 0; //runs the algorithm
	virtual string ResultString(); //generates the result and returns it
	virtual void DoDeduction(); //do the deduction of the task
	virtual void AddAxiomsToSigma(); //add the axioms to sigma for easier iteration
};

#endif