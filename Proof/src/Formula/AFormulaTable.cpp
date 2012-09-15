#include "AFormulaTable.h"
#include "FalseFormula.h"
#include "TempFormula.h"

namespace AFormulaTable
{
	hash_map<unsigned, AtomicFormula*> table;

	AtomicFormula * GetAtomicFormula(char * symbol)
	{
		int hash = 0;
		int len = strlen(symbol);

		hash = 31 * hash + 0;

		for(int i = 0; i < len; i++)
		{
			hash = 31 * hash + symbol[ i ];
		}

		return table[hash];
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