#include "AFormulaTable.h"
#include "FalseFormula.h"
#include "TempFormula.h"

using std::hash;

namespace AFormulaTable
{
	static hash_map<unsigned, AtomicFormula*> table;
	static hash_map<string, unsigned> charTable;
	static unsigned lastId = 0;

	AtomicFormula * GetAtomicFormula(char * symbol)
	{
		unsigned id = charTable[symbol];
		if(id == 0)
			return NULL;
		return table[ id ];
	}

	AtomicFormula * GetTempFormula(char * symbol)
	{
		stringstream stream;
		stream<<'_'<<symbol<<'\0';
		int len = stream.str().length();
		symbol = new char[len];
		stream.str().copy(symbol, len);

		return GetAtomicFormula(symbol);
	}

	AtomicFormula * GetAtomicFormula(unsigned id)
	{
		return table[id];
	}

	AtomicFormula * AddAtomicFormula(AtomicFormula * formula)
	{
		AtomicFormula * ret = GetAtomicFormula(formula->GetSymbol());

		if(ret == NULL)
		{
			formula->SetId(++lastId);
			ret = formula;
			table[lastId] = ret;
			charTable[ ret->GetSymbol() ] = lastId;
		}
		else
		{
			delete formula;
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