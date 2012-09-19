#pragma once

#include "ImplicationFormula.h"

class Axiom : public ImplicationFormula
{
public:
	Axiom();
	Axiom(IFormula * left, IFormula * right);
	Axiom(Axiom& axiom);
	~Axiom() {}

	IFormula * Clone();
};