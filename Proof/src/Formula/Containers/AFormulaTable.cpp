#include "AFormulaTable.h"
#include "../Atomic/FalseFormula.h"

namespace AFormulaTable
{
	static hash_map<long, spAtomicFormula> table;
	static hash_map<string, long> charTable;

	spAtomicFormula GetAtomicFormula(char * symbol)
	{
		hash_map<string, long>::iterator it = charTable.find(symbol);
		if(it == charTable.end())
			return __nullptr;
		return table[ it->second ];
	}

	spTempFormula GetTempFormula(char * symbol)
	{
		stringstream stream;
		stream<<'_'<<symbol<<'\0';
		int len = stream.str().length();
		symbol = new char[len];
		strcpy_s(symbol, len, stream.str().c_str());

		return static_pointer_cast<TempFormula>(GetAtomicFormula(symbol));
	}

	spAtomicFormula GetAtomicFormula(long hash)
	{
		return table[hash];
	}

	spAtomicFormula AddAtomicFormula(AtomicFormula * formula)
	{
		spAtomicFormula ret = GetAtomicFormula(formula->GetSymbol());

		if(ret == __nullptr)
		{
			long hash = formula->HashCode();
			ret = spAtomicFormula(formula);
			table[ hash ] = ret;
			charTable[ ret->GetSymbol() ] = hash;
		}

		return ret;
	}

	spAtomicFormula AddAtomicFormula(char * symbol)
	{
		spAtomicFormula ret = GetAtomicFormula(symbol);

		if(ret == __nullptr)
			ret = AddAtomicFormula(new AtomicFormula(symbol));

		return ret;
	}

	spTempFormula AddTempFormula(char * symbol)
	{
		spAtomicFormula ret = GetTempFormula(symbol);

		if(ret == __nullptr)
			ret = AddAtomicFormula(new TempFormula(symbol));

		return static_pointer_cast<TempFormula>(ret);
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
		charTable.clear();
		table.clear();
	}

	string ListAtomicFormulas()
	{
		stringstream stream;
		for(auto it : table)
		{
			stream << it.second->ToString() << "\t" << it.second->HashCode() << "\t" << (it.second->IsTemp()?"Temp":"") << "\r\n";
		}
		stream<<'\0';
		return stream.str();
	}

	hash_map<long, spAtomicFormula>::iterator TableBegin()
	{
		return table.begin();
	}

	hash_map<long, spAtomicFormula>::iterator TableEnd()
	{
		return table.end();
	}
}