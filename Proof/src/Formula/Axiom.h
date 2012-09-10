#pragma once

#include "CompoundFormula.h"

class AtomicFormula;

class Axiom : public CompoundFormula
{
public:
	Axiom() {}
	/*Axiom(vector<IFormula*> formulas)
		: CompoundFormula(formulas)
	{
	}*/
	Axiom(IFormula * formulas[])
		: CompoundFormula(formulas)
	{
	}
	~Axiom();

	bool IsAtomic();
	bool Eval();
	string ToString();
	CompoundFormula * Replace(string x, string t); //Replace the axiom's atomic formula x in every occourance with t
	CompoundFormula * Replace(string x, AtomicFormula t);
	CompoundFormula * Replace(AtomicFormula x, AtomicFormula t);
};