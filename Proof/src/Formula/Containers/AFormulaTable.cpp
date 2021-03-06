#include "AFormulaTable.h"
#include "../Atomic/FalseFormula.h"

namespace AFormulaTable
{
	using std::endl;

	static unordered_map<long, spAtomicFormula> table;

	spAtomicFormula GetAtomicFormula(char * symbol)
	{
		long hash = GenerateHashCode(symbol);

		return table[hash];
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

		delete[] symbol;

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
		}

		return ret;
	}

	spAtomicFormula AddAtomicFormula(char * symbol)
	{
		spAtomicFormula ret = GetAtomicFormula(symbol);

		if(ret == nullptr)
		{
			if(symbol == FALSE)
				ret = AddAtomicFormula(new FalseFormula());
			else
				ret = AddAtomicFormula(new AtomicFormula(symbol));
		}

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
		AddAtomicFormula(new FalseFormula());

		AddAtomicFormula(new TempFormula("a"));
		AddAtomicFormula(new TempFormula("b"));
		AddAtomicFormula(new TempFormula("c"));
	}

	void DestroyTable()
	{
		table.clear();
	}

	string ListAtomicFormulas()
	{
		stringstream stream;
		for(auto it : table)
		{
			stream << it.second->ToString() << "\t" << it.second->HashCode() << "\t" << (it.second->IsTemp()?"Temp":"") << endl;
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