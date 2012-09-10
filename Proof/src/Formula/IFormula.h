#pragma once
#include "../HilbertIncludes.h"

/**
*	Formula interface
*/
class IFormula
{
public:
	IFormula() {}
	virtual bool IsAtomic() { return true; }
	virtual bool IsTemp() { return false; }
	virtual bool Eval() { return true; }
	virtual bool Equals(IFormula * formula) { return false; }
	virtual char * ToString() { return ""; }
};