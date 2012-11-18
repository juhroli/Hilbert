#ifndef AFormulaTable_h
#define AFormulaTable_h

#include "../../HilbertIncludes.h"
#include "../Atomic/AtomicFormula.h"
#include "../Atomic/TempFormula.h"
#include <unordered_map>

using std::unordered_map;
using std::static_pointer_cast;

/*
*	Namespace for storing atomic formulas (including temp and false formulas).
*/
namespace AFormulaTable
{
	spAtomicFormula GetAtomicFormula(char * symbol);
	spTempFormula GetTempFormula(char * symbol);
	spAtomicFormula GetAtomicFormula(long hash);
	spAtomicFormula AddAtomicFormula(AtomicFormula * formula);
	spAtomicFormula AddAtomicFormula(char * symbol);
	spTempFormula AddTempFormula(char * symbol);

	void SetDefaults();
	void DestroyTable();
	string ListAtomicFormulas();

	unordered_map<long, spAtomicFormula>::iterator TableBegin();
	unordered_map<long, spAtomicFormula>::iterator TableEnd();
}

#endif