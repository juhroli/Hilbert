#include "Axiom.h"

Axiom::~Axiom()
{

}

bool Axiom::IsAtomic()
{
	return CompoundFormula::IsAtomic();
}

bool Axiom::Eval()
{
	return CompoundFormula::Eval();
}

string Axiom::ToString()
{
	return CompoundFormula::ToString();
}