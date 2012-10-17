#include <iostream>

#include "HilbertIncludes.h"
#include "Formula/Atomic/AtomicFormula.h"
#include "Formula/Atomic/TempFormula.h"
#include "Formula/Compound/ImplicationFormula.h"
#include "Formula/Compound/Axiom.h"
#include "Formula/Containers/AFormulaTable.h"
#include "Formula/Containers/HilbertAxioms.h"
#include "Input/FormulaParser.h"
#include "Formula/Containers/FormulaSet.h"

#include <string>

using namespace AFormulaTable;
using namespace FormulaParser;
using namespace std;

int main(int argc, char* argv[])
{
	/*
	*	Arguments:
	*	0: all tests
	*	1: old replace
	*	2: new replace
	*	3: old StringToFormula
	*	4: new ParseFormula
	*	5: FormulaSet
	*/

	/*=================TEST FOR REPLACE (OLD VERSION)=================*/
	//if( argc > 1 && (atoi(argv[1]) == 1 || atoi(argv[1]) == 0))
	//{
	//	AddTempFormula("F");
	//	AddTempFormula("G");
	//	AddTempFormula("H");
	//	//(F -> (G -> H)) -> ((F -> G) -> (F -> H))
	//	Axiom * ax = new Axiom(new Axiom(GetTempFormula("F"), new Axiom(GetTempFormula("G"), GetTempFormula("H"))),
	//		new Axiom(new Axiom(GetTempFormula("F"), GetTempFormula("G")), new Axiom(GetTempFormula("F"), GetTempFormula("H"))));
	//	AddAtomicFormula("F");
	//	AddAtomicFormula("G");
	//	AddAtomicFormula("H");

	//	for(int i = 0; i < 100; i++)
	//	{
	//		ImplicationFormula * f1 = ax->Replace2(GetTempFormula("F"), GetAtomicFormula("F"));
	//		ImplicationFormula * f2 = f1->Replace2(GetTempFormula("G"), GetAtomicFormula("G"));
	//		ImplicationFormula * f3 = f2->Replace2(GetTempFormula("H"), GetAtomicFormula("H"));

	//		delete f3;
	//		delete f2;
	//		delete f1;
	//	}
	//	delete ax;
	//}

	/*=================TEST FOR REPLACE (NEW VERSION)=================*/
	if( argc > 1 && (atoi(argv[1]) == 2 || atoi(argv[1]) == 0))
	{
		AddTempFormula("F");
		AddTempFormula("G");
		AddTempFormula("H");
		//(F -> (G -> H)) -> ((F -> G) -> (F -> H))
		IFormula * ax = ParseTemp("(F -> (G -> H)) -> ((F -> G) -> (F -> H))");
		AddAtomicFormula("F");
		AddAtomicFormula("G");
		AddAtomicFormula("H");

		for(int i = 0; i < 100; i++)
		{
			IFormula * f1 = ax->Replace(*GetTempFormula("F"), *GetAtomicFormula("F"));
			IFormula * f2 = f1->Replace(*GetTempFormula("G"), *GetAtomicFormula("G"));
			IFormula * f3 = f2->Replace(*GetTempFormula("H"), *GetAtomicFormula("H"));

			delete f3;
			delete f2;
			delete f1;
		}
		delete ax;
	}

	/*=================TEST FOR STRING TO FORMULA (OLD VERSION)=================*/
	/*if( argc > 1 && (atoi(argv[1]) == 3 || atoi(argv[1]) == 0))
	{
		for(int i = 0; i < 100; i++)
		{
			IFormula * f = StringToFormula("(F -> (G -> H)) -> ((F -> G) -> (F -> H))");
			delete f;
			f = StringToFormula("(F -> G -> H) -> ((F -> G) -> F -> H)");
			delete f;
			f = StringToFormula("F -> ( G -> H ) -> ( G->X ) -> ( F -> X -> Y )");
			delete f;
		}
	}*/

	/*=================TEST FOR PARSE FORMULA (NEW VERSION)=================*/
	if( argc > 1 && (atoi(argv[1]) == 4 || atoi(argv[1]) == 0))
	{
		for(int i = 0; i < 100; i++)
		{
			IFormula * f = ParseFormula("(F -> (G -> H)) -> ((F -> G) -> (F -> H))");
			delete f;
			f = ParseFormula("(F -> G -> H) -> ((F -> G) -> F -> H)");
			delete f;
			f = ParseFormula("F -> ( G -> H ) -> ( G->X ) -> ( F -> X -> Y )");
			delete f;
		}
	}

	/*=================TEST FOR FORMULA SET=================*/
	if( argc > 1 && (atoi(argv[1]) == 5 || atoi(argv[1]) == 0))
	{
		FormulaSet fset;
		for(int i = 1; i <= 100; i++)
		{
			stringstream stream;
			stream<<"(F"<<i<<" -> (G"<<i<<" -> H"<<i<<")) -> ((F"<<i<<" -> G"<<i<<") -> (F"<<i<<" -> H"<<i<<"))";
			fset.Add(ParseFormula(stream.str()));
		}

		/*IFormula * f = ParseFormula("F");
		fset.Add(f);
		cout<<ListAtomicFormulas()<<endl;
		DELETEFORMULA(f);
		cout<<ListAtomicFormulas()<<endl;*/

		/*concurrent_vector<IFormula*>& vec = fset.GetFormulas();
		hash_map<long, pair<IFormula*, bool>>& hashm = fset.GetMap();
		vec = fset.GetFormulas();
		hashm = fset.GetMap();

		for(auto it : vec)
		{
			cout<<it->ToString()<<endl;
		}

		fset.SortFormulas();

		for(auto it : vec)
		{
			cout<<it->ToString()<<" : "<<it->Length()<<endl;
		}*/

		/*for(auto x : hashm)
		{
			cout<<x.second.first->ToString()<<endl;
		}*/
	}
	
	//SetDefaults();
	//HilbertAxioms axioms;
	//cout<<"Hilbert axioms:"<<endl;
	//for(int i = 1; i <= 3; i++)
	//{
	//	cout<<axioms.GetAxiom(i)->ToString()<<endl;
	//}

	//string str;
	//
	//while(true)
	//{
	//	cout<<"List of atomic formulas:"<<endl<<ListAtomicFormulas()<<endl;
	//	cout<<"Write a formula: "<<endl;
	//	str.clear();
	//	cin.clear();
	//	getline(cin, str);
	//	IFormula * form = ParseFormula(str);
	//	if(form != NULL)
	//	{
	//		cout<<form->ToString()<<endl;
	//	}
	//	else
	//	{
	//		cout<<"Syntax error."<<endl;
	//	}

	//	if(form != NULL && !form->IsAtomic()) delete form;
	//	

	//	cout<<"Type e to exit or press ENTER to continue...";
	//	if(cin.get() == 'e')
	//		break;
	//}
	DestroyTable();

	return 0;
}