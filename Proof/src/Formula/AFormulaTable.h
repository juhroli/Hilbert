#pragma once

#include "AtomicFormula.h"
#include <hash_map>

using std::hash_map;

/*
*	Namespace for storing atomic formulas (including temp and false formulas).
*/
namespace AFormulaTable
{
	AtomicFormula * GetAtomicFormula(char * symbol);
	AtomicFormula * GetTempFormula(char * symbol);
	AtomicFormula * GetAtomicFormula(unsigned hash);
	AtomicFormula * AddAtomicFormula(AtomicFormula * formula);
	void SetDefaults();
	void DestroyTable();
}