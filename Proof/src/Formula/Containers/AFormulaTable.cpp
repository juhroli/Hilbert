#include "AFormulaTable.h"
#include "../Atomic/FalseFormula.h"

namespace AFormulaTable
{
	static unordered_map<long, spAtomicFormula> table;
	static unordered_map<string, long> charTable;

	spAtomicFormula GetAtomicFormula(char * symbol)
	{
		unordered_map<string, long>::iterator it = charTable.find(symbol);
		if(it == charTable.end())
			return nullptr;
		return table[ it->second ];
	}

	spTempFormula GetTempFormula(char * symbol)
	{
		stringstream stream;
		stream<<'_'<<symbol<<'\0';
		int len = stream.str().length();
		symbol = new char[len];

#ifdef _MSC_VER
		strcpy_s(symbol, len, stream.str().c_str());
#else
		strcpy(symbol, stream.str().c_str());
#endif

		auto ret = static_pointer_cast<TempFormula>(GetAtomicFormula(symbol));
		
		delete symbol;

		return ret;
	}

	spAtomicFormula GetAtomicFormula(long hash)
	{
		return table[hash];
	}

	spAtomicFormula AddAtomicFormula(AtomicFormula * formula)
	{
		spAtomicFormula ret = GetAtomicFormula(formula->GetSymbol());

		if(ret == nullptr)
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

		if(ret == nullptr)
			ret = AddAtomicFormula(new AtomicFormula(symbol));

		return ret;
	}

	spTempFormula AddTempFormula(char * symbol)
	{
		spAtomicFormula ret = GetTempFormula(symbol);

		if(ret == nullptr)
			ret = AddAtomicFormula(new TempFormula(symbol));

		return static_pointer_cast<TempFormula>(ret);
	}

	void SetDefaults()
	{
		//TODO
		AddAtomicFormula(new FalseFormula());
		AddAtomicFormula(new AtomicFormula("F"));
		AddAtomicFormula(new AtomicFormula("G"));
		AddAtomicFormula(new AtomicFormula("H"));

		AddAtomicFormula(new TempFormula("a"));
		AddAtomicFormula(new TempFormula("b"));
		AddAtomicFormula(new TempFormula("c"));
		AddAtomicFormula(new TempFormula("d"));
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

	unordered_map<long, spAtomicFormula>::iterator TableBegin()
	{
		return table.begin();
	}

	unordered_map<long, spAtomicFormula>::iterator TableEnd()
	{
		return table.end();
	}
}