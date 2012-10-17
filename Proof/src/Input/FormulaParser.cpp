#include "FormulaParser.h"

#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Compound/Axiom.h"
#include "../Formula/Containers/AFormulaTable.h"

namespace FormulaParser
{
	using namespace AFormulaTable;
	
	/*
	*
	*/
	IFormula * ParseFormula(string str)
	{
		string::iterator& it = str.begin();
		string::iterator& end = str.end();

		IFormula * ret = ReadFormula(it, end, false);
		return (it == end ? ret : __nullptr);
	}
	
	/*
	*
	*/
	IFormula * ParseTemp(string str)
	{
		string::iterator& it = str.begin();
		string::iterator& end = str.end();

		IFormula * ret = ReadFormula(it, end, true);
		return (it == end ? ret : __nullptr);
	}
	
	/*
	*
	*/
	IFormula * ReadFormula(string::iterator& it, string::iterator& end, bool temp)
	{
		IFormula * ret = ReadSingleFormula(it, end, temp);
		while(it != end && *it == ' ')
		{
			it++;
		}
		if(ret && it != end && *it == '-')
		{
			it++;
			if(it != end && *it != '>')
				return __nullptr;
			IFormula * right = ReadFormula(++it, end, temp);

			if(!right)
				return __nullptr;

			ret = (temp ? new Axiom(ret, right) : new ImplicationFormula(ret, right));
		}
		return ret;
	}

	/*
	*
	*/
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end, bool temp)
	{
		while(it != end && *it == ' ')
		{
			it++;
		}
		if(it == end) return __nullptr;
		char ch = *(it++);
		
		
		if(ch == *FALSE)
			return AddAtomicFormula(FALSE).get();

		if(ch == '(')
		{
			IFormula * ret = ReadFormula(it, end, temp);
			ch = *(it++);
			return (ch == ')' ? ret : __nullptr);
		}

		if(ch == '_' || (ch >= '0' && ch <= '9')
			|| (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			stringstream stream;
			stream<<ch;
			while(it != end && ( *it == '_' || (*it >= '0' && *it <= '9')
				|| (*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z') ))
			{
				stream<<*(it++);
			}
			stream<<'\0';

			int len = stream.str().length();
			char * cStr = new char[len];
			strcpy_s(cStr, len, stream.str().c_str());

			IFormula * ret = (temp ? AddTempFormula(cStr).get() : AddAtomicFormula(cStr).get());
			delete[] cStr;
			return ret;
		}
		
		return __nullptr;
	}
 
}