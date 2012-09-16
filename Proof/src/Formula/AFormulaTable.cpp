#include "AFormulaTable.h"
#include "FalseFormula.h"
#include "TempFormula.h"

namespace AFormulaTable
{
	hash_map<unsigned, AtomicFormula*> table;

	unsigned Hash(char * str)
	{
		unsigned hash = 0;
		unsigned len = strlen(str);

		for(unsigned i = 0; i < len; i++)
		{
			hash = 31 * hash + str[ i ];
		}

		hash = 31 * hash;

		return hash;
	}

	AtomicFormula * GetAtomicFormula(char * symbol)
	{
		return table [Hash(symbol) ];
	}

	AtomicFormula * GetTempFormula(char * symbol)
	{
		return table[ Hash(symbol) + 1 ];
	}

	AtomicFormula * GetAtomicFormula(unsigned hash)
	{
		return table[hash];
	}

	AtomicFormula * AddAtomicFormula(AtomicFormula * formula)
	{
		unsigned hash = formula->GetHash();

		AtomicFormula * ret = GetAtomicFormula(hash);
		if(ret == NULL)
		{
			ret = formula;
			table[hash] = ret;
		}
		return ret;
	}

	void SetDefaults()
	{
		//TODO
		AddAtomicFormula(new FalseFormula());
		AddAtomicFormula(new AtomicFormula("F"));
		AddAtomicFormula(new TempFormula("F"));
		AddAtomicFormula(new AtomicFormula("G"));
		AddAtomicFormula(new TempFormula("G"));
		AddAtomicFormula(new AtomicFormula("H"));
		AddAtomicFormula(new TempFormula("H"));
	}

	void DestroyTable()
	{
		table.clear();
	}
}