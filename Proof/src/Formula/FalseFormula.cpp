#include "FalseFormula.h"

bool FalseFormula::IsAtomic()
{
	return AtomicFormula::IsAtomic();
}

bool FalseFormula::Eval()
{
	return false;
}

string FalseFormula::ToString()
{
	return AtomicFormula::ToString();
}