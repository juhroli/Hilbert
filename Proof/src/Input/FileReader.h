#ifndef FileReader_h
#define FileReader_h

#include "../HilbertIncludes.h"
#include "FormulaParser.h"

class FileReader
{
public:
	FileReader();
	FileReader(string file);
	FileReader(string file, FSetType type);
	~FileReader();

	bool ReadFile();

	IFormula*& GetTarget();
	IFormulaSet*& GetSet();
	AxiomContainer*& GetAxioms();

private:
	string m_file;
	IFormula * m_target;
	IFormulaSet * m_set;
	AxiomContainer * m_axioms;
	FSetType m_setType;
};

#endif