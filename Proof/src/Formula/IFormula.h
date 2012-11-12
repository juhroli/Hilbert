#pragma once

#include "../HilbertIncludes.h"
#include <sstream>
#include <locale>

using std::locale;
using std::collate;
using std::use_facet;
using std::stringstream;
using std::string;

static long GenerateHashCode(string str)
{
	static locale loc;
	static const collate<char>& coll = use_facet<collate<char>>(loc);
	return coll.hash(str.data(), str.end()._Ptr);
}


/*
*	Formula interface
*/
class IFormula
{
public:
	virtual ~IFormula() {}
	virtual bool IsAtomic() = 0;
	virtual bool IsTemp() = 0;
	virtual bool Eval() = 0;
	virtual bool Equals(IFormula * formula) = 0;
	virtual string ToString() = 0;
	virtual IFormula * Clone() = 0;
	virtual bool IsNull() = 0;
	virtual IFormula * Replace(IFormula * t, IFormula * x) = 0;
	virtual unsigned Length() = 0;
	virtual long HashCode() = 0;
};