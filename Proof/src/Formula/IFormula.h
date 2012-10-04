#pragma once
#include "../HilbertIncludes.h"
#include <sstream>

using std::stringstream;
using std::string;

/*
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
	virtual string ToString() { return ""; }
	virtual IFormula * Clone() { return NULL; }
	virtual bool IsNull() { return true; }
	virtual IFormula * Replace(IFormula& t, IFormula& x) { return NULL; }
};