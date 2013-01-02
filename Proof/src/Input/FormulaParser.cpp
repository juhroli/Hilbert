#include "FormulaParser.h"

#include "../Formula/Compound/ImplicationFormula.h"
#include "../Formula/Compound/Axiom.h"
#include "../Formula/Containers/AFormulaTable.h"
#include "../Formula/Containers/Sets/FormulaSetList.h"
#include "../Formula/Containers/Sets/FormulaSetVector.h"
#include "../Algorithm/General.h"

namespace FormulaParser
{
	using namespace AFormulaTable;
	using General::NormalizeFormula;
	
	/*
	*	This creates an AtomicFormula
	*	or ImplicationFormula from a string.
	*/
	IFormula * ParseFormula(string str)
	{
		string::iterator it = str.begin();
		string::iterator end = str.end();

		IFormula * ret = ReadFormula(it, end, false);
		
		if(it != end && *it != ' ')
		{
			DELETEFORMULA(ret);
		}

		NormalizeFormula(ret);

		return ret;
	}
	
	/*
	*	This creates a TempFormula
	*	or Axiom from a string.
	*/
	IFormula * ParseTemp(string str)
	{
		string::iterator it = str.begin();
		string::iterator end = str.end();

		IFormula * ret = ReadFormula(it, end, true);

		if(it != end && *it != ' ')
		{
			DELETEFORMULA(ret);
		}

		return ret;
	}
	
	/*
	*	Reads a formula which can be either
	*	atomic or compound.
	*	@param temp defines if the formula consinst only of TempFormula(s)
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
			{
				DELETEFORMULA(ret);
				return nullptr;
			}

			IFormula * right = ReadFormula(++it, end, temp);

			if(!right)	//Something's not right here...
			{
				DELETEFORMULA(ret);
				return nullptr;
			}

			ret = (temp ? new Axiom(ret, right) : new ImplicationFormula(ret, right));
			right = nullptr;
		}

		return ret;
	}

	/*
	*	Reads an atomic formula.
	*	@param temp defines if the formula is a TempFormula
	*/
	IFormula * ReadSingleFormula(string::iterator& it, string::iterator& end, bool temp)
	{
		while(it != end && *it == ' ')
		{
			it++;
		}

		if(it == end)
			return nullptr;

		char ch = *(it++);
		
		
		if(ch == *FALSE)
			return AddAtomicFormula(FALSE).get();

		if(ch == '(')
		{
			IFormula * ret = ReadFormula(it, end, temp);
			ch = *(it++);
			
			if(ch != ')')
			{
				DELETEFORMULA(ret);
			}

			return ret;
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

#ifdef _MSC_VER
			strcpy_s(cStr, len, stream.str().c_str());
#else
			strcpy(cStr, stream.str().c_str());
#endif

			IFormula * ret = (temp ? AddTempFormula(cStr).get() : AddAtomicFormula(cStr).get());
			delete[] cStr;
			return ret;
		}
		
		return nullptr;
	}

	/*
	*	Converts a string into a set of formulas.
	*	@param type is the type of the set that should be used;
	*				FSetType is defined in IFormulaSet's header
	*/
	IFormulaSet * ParseFormulaSet(string str, FSetType type)
	{
		IFormulaSet * ret = nullptr;

		switch(type)
		{
		case FSET_LIST:
			ret = new FormulaSetList();
			break;
		case FSET_VEC:
			ret = new FormulaSetVector();
		}
		
		auto it = str.begin();
		auto end = str.end();

		while(it != end && *it == ' ')
			it++;

		if( *it != '{' || !ReadFormulaSet(++it, end, false, ret) )
		{
			delete ret;
			ret = nullptr;
		}

		return ret;
	}

	/*
	*	Converts a string into an AxiomContainer.
	*/
	AxiomContainer * ParseAxioms(string str)
	{
		AxiomContainer * ret = new AxiomContainer();
		
		auto it = str.begin();
		auto end = str.end();

		while(it != end && *it == ' ')
			it++;

		if( *it != '{' || !ReadFormulaSet(++it, end, true, ret) )
		{
			delete ret;
			ret = nullptr;
		}

		return ret;
	}

	/*
	*	Reads a set of formulas from a string
	*	starting after the '{' character.
	*
	*	@param formulas the type of it is defined
	*	by the caller, it can either be an IFormulaSet
	*	or an AxiomContainer. Formulas are added to it.
	*	@param temp whether create temp formulas or not
	*/
	template<typename T>
	bool ReadFormulaSet(string::iterator& it, string::iterator& end, bool temp, T * formulas)
	{
		IFormula * F = ReadFormula(it, end, temp);

		if(!F || it == end)
			return false;

		while(it != end && *it == ' ')
			it++;

		if(*it != ',' && *it != '}')
		{
			DELETEFORMULA(F);
			return false;
		}

		/*
		*	Warning:
		*	If T doesn't have an Add(IFormula*) method,
		*	compiling will fail.
		*/
		formulas->Add(F);

		if(*it == '}')
			return true;

		return ReadFormulaSet(++it, end, temp, formulas);
	}
 
}