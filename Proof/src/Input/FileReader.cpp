#include "FileReader.h"

#include <fstream>
#include "../Formula/Containers/HilbertAxioms.h"

using namespace FormulaParser;

FileReader::FileReader()
	: m_file("")
	, m_setType(FSET_LIST)
	, m_target(nullptr)
	, m_axioms(nullptr)
	, m_set(nullptr)
{
}

FileReader::FileReader(string file)
	: m_file(file)
	, m_setType(FSET_LIST)
	, m_target(nullptr)
	, m_axioms(nullptr)
	, m_set(nullptr)
{
}

FileReader::FileReader(string file, FSetType type)
	: m_file(file)
	, m_setType(type)
	, m_target(nullptr)
	, m_axioms(nullptr)
	, m_set(nullptr)
{
}

FileReader::~FileReader()
{
	DELETEFORMULA(m_target);

	if(m_set != nullptr)
	{
		delete m_set;
		m_set = nullptr;
	}

	if(m_axioms != nullptr)
	{
		delete m_axioms;
		m_axioms = nullptr;
	}
}

/*
*	Reads a file that should look like this:
*
*	1. line: "{ axiom_1, ..., axiom_n }" OR "default" //default means to use Hilbert axioms
*	2. line: "formula" //this is the target formula
*	3. line: "{ formula_1, ..., formula_n }" //OR leave this line empty; this conatains the formula set
*
*	Quote marks are not needed.
*
*/
bool FileReader::ReadFile()
{
	ifstream input(m_file);

	if(input.is_open())
	{
		string str;
		int line = 0;

		while(!input.eof())
		{
			getline(input, str); //reads the current line into str
			
			//if the current line is empty, ignore it
			if(str.empty())
				continue;
			
			switch(line)
			{
			case 0:
				if(str.compare("default") == 0)
					m_axioms = new HilbertAxioms();
				else
					if( !(m_axioms = ParseAxioms(str)) )
						return false;
				break;

			case 1:
				if( !(m_target = ParseFormula(str)) )
					return false;
				break;
			case 2:
				if( !(m_set = ParseFormulaSet(str, m_setType)) )
					return false;
			}
			
			line++;
		}
	}
	else
	{
		return false;
	}

	return true;
}

IFormula*& FileReader::GetTarget()
{
	return m_target;
}

IFormulaSet*& FileReader::GetSet()
{
	return m_set;
}

AxiomContainer*& FileReader::GetAxioms()
{
	return m_axioms;
}