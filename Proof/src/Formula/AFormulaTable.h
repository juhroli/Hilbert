#pragma once

#include "AtomicFormula.h"
#include "TempFormula.h"
#include <hash_map>

using std::hash_map;

/*
*	Namespace for storing atomic formulas (including temp and false formulas).
*/
namespace AFormulaTable
{
	AtomicFormula * GetAtomicFormula(char * symbol);
	TempFormula * GetTempFormula(char * symbol);
	AtomicFormula * GetAtomicFormula(unsigned hash);
	AtomicFormula * AddAtomicFormula(AtomicFormula * formula);
	AtomicFormula * AddAtomicFormula(char * symbol);
	TempFormula * AddTempFormula(char * symbol);
	void SetDefaults();
	void DestroyTable();
	string ListAtomicFormulas();
}