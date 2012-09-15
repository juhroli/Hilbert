#pragma once

#include "ImplicationFormula.h"

class TempFormula;

class Axiom : public ImplicationFormula
{
public:
	Axiom();
	Axiom(IFormula * left, IFormula * right);
	Axiom(Axiom& axiom);
	~Axiom();

	IFormula * Clone();
	
	Axiom * Replace(char * x, IFormula * t); //Replace the axiom's temp atomic formula x in every occourance with t
	Axiom * Replace(TempFormula * x, IFormula * t);
};