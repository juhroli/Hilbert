#include <iostream>

#include "HilbertIncludes.h"
#include "Formula/AtomicFormula.h"
#include "Formula/TempFormula.h"
#include "Formula/ImplicationFormula.h"
#include "Formula/Axiom.h"
#include "Formula/AFormulaTable.h"
#include "Formula/HilbertAxioms.h"
#include "Input/InputHandler.h"
#include "Input/FormulaParser.h"

#include <string>

using namespace InputHandler;
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
	*/

	/*=================TEST FOR REPLACE (OLD VERSION)=================*/
	if( argc > 1 && (atoi(argv[1]) == 1 || atoi(argv[1]) == 0))
	{
		AddTempFormula("F");
		AddTempFormula("G");
		AddTempFormula("H");
		//(F -> (G -> H)) -> ((F -> G) -> (F -> H))
		Axiom * ax = new Axiom(new Axiom(GetTempFormula("F"), new Axiom(GetTempFormula("G"), GetTempFormula("H"))),
			new Axiom(new Axiom(GetTempFormula("F"), GetTempFormula("G")), new Axiom(GetTempFormula("F"), GetTempFormula("H"))));
		AddAtomicFormula("F");
		AddAtomicFormula("G");
		AddAtomicFormula("H");

		for(int i = 0; i < 100; i++)
		{
			ImplicationFormula * f1 = ax->Replace2(GetTempFormula("F"), GetAtomicFormula("F"));
			ImplicationFormula * f2 = f1->Replace2(GetTempFormula("G"), GetAtomicFormula("G"));
			ImplicationFormula * f3 = f2->Replace2(GetTempFormula("H"), GetAtomicFormula("H"));

			delete f3;
			delete f2;
			delete f1;
		}
		delete ax;
	}

	/*=================TEST FOR REPLACE (NEW VERSION)=================*/
	if( argc > 1 && (atoi(argv[1]) == 2 || atoi(argv[1]) == 0))
	{
		AddTempFormula("F");
		AddTempFormula("G");
		AddTempFormula("H");
		//(F -> (G -> H)) -> ((F -> G) -> (F -> H))
		Axiom * ax = new Axiom(new Axiom(GetTempFormula("F"), new Axiom(GetTempFormula("G"), GetTempFormula("H"))),
			new Axiom(new Axiom(GetTempFormula("F"), GetTempFormula("G")), new Axiom(GetTempFormula("F"), GetTempFormula("H"))));
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
	if( argc > 1 && (atoi(argv[1]) == 3 || atoi(argv[1]) == 0))
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
	}

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



	//SetDefaults();
	//HilbertAxioms axioms;
	//cout<<"Hilbert axioms:"<<endl;
	//for(int i = 1; i <= 3; i++)
	//{
	//	cout<<axioms.GetAxiom(i)->ToString()<<endl;
	//}
	//IFormula * ax = (axioms.GetAxiom(1)->Replace(*GetTempFormula("F"), *AddAtomicFormula("Z")));
	//cout<<ax->ToString()<<endl;
	////cout<<ax->Replace2("G", new AtomicFormula("U"))->ToString()<<endl;
	//delete ax;

	////TODO: process these kind of formulas

	//IFormula * form = ParseFormula("F->(F->G)->(F->H)");
	//if(form != NULL)
	//{
	//	cout<<form->ToString()<<endl;
	//}
	//delete form;

	//form = ParseFormula("F->G->H->G->H");
	//if(form != NULL)
	//{
	//	cout<<form->ToString()<<endl;
	//}
	//delete form;

	//form = ParseFormula("(F->G->H)->((F->G)->(F->H))");
	//if(form != NULL)
	//{
	//	cout<<form->ToString()<<endl;
	//}
	//delete form;

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