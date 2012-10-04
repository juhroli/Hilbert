#include "FormulaParser.h"
#include "../Formula/ImplicationFormula.h"
#include "../Formula/AFormulaTable.h"

namespace FormulaParser
{
	using namespace AFormulaTable;

	IFormula * ParseFormula(string str)
	{
		string::iterator& it = str.begin();
		string::iterator& end = str.end();

		IFormula * ret = ReadFormula(it, end);
		return (it == end ? ret : NULL);
	}

	IFormula * ReadFormula(string::iterator& it, string::iterator& end)
	{
		IFormula * ret = ReadSingleFormula(it, end);
		while(it != end && *it == ' ')
		{
			it++;
		}
		if(ret && it != end && *it == '-')
		{
			it++;
			if(it != end && *it != '>')
				return NULL;
			IFormula * right = ReadFormula(++it, end);

			if(!right)
				return NULL;

			ret = new ImplicationFormula(ret, right);
		}
		return ret;
	}

	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end)
	{
		while(it != end && *it == ' ')
		{
			it++;
		}
		if(it == end) return NULL;
		char ch = *(it++);
		
		
		if(ch == *FALSE)
			return AddAtomicFormula(FALSE);

		if(ch == '(')
		{
			IFormula * ret = ReadFormula(it, end);
			ch = *(it++);
			return (ch == ')' ? ret : NULL);
		}

		if(ch == '_' || (ch >= 0 && ch <= 9)
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
			char * cStr = new char[stream.str().length()];
			stream.str().copy(cStr, stream.str().length());

			return AddAtomicFormula(cStr);
		}

		return NULL;
	}
 
}