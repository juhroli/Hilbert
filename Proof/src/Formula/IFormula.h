#ifndef IFormula_h
#define IFormula_h

#include "../HilbertIncludes.h"
#include <sstream>
#include <locale>
#include <cstring>

using std::locale;
using std::collate;
using std::use_facet;
using std::stringstream;
using std::string;
using std::strlen;

static long GenerateHashCode(string str)
{
	static locale loc;
	static const collate<char>& coll = use_facet<collate<char>>(loc);

	//For some reason this doesn't work in VS debug mode...
#ifndef _DEBUG
	return coll.hash(str.data(), &*str.end());
#else
	return coll.hash(str.data(), str.end()._Ptr);
#endif
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
	virtual bool IsWrapped() = 0;
};

#endif