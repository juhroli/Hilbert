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

int main()
{
	SetDefaults();
	HilbertAxioms axioms;
	cout<<"Hilbert axioms:"<<endl;
	for(int i = 1; i <= 3; i++)
	{
		cout<<axioms.GetAxiom(i)->ToString()<<endl;
	}
	IFormula * ax = (axioms.GetAxiom(1)->Replace(*GetTempFormula("F"), *AddAtomicFormula("Z")));
	cout<<ax->ToString()<<endl;
	//cout<<ax->Replace2("G", new AtomicFormula("U"))->ToString()<<endl;
	delete ax;

	//TODO: process these kind of formulas

	IFormula * form = ParseFormula("F->(F->G)->(F->H)");
	if(form != NULL)
	{
		cout<<form->ToString()<<endl;
	}
	delete form;

	form = ParseFormula("F->G->H->G->H");
	if(form != NULL)
	{
		cout<<form->ToString()<<endl;
	}
	delete form;

	form = ParseFormula("(F->G->H)->((F->G)->(F->H))");
	if(form != NULL)
	{
		cout<<form->ToString()<<endl;
	}
	delete form;

	string str;
	
	while(true)
	{
		cout<<"List of atomic formulas:"<<endl<<ListAtomicFormulas()<<endl;
		cout<<"Write a formula: "<<endl;
		str.clear();
		cin.clear();
		getline(cin, str);
		IFormula * form = ParseFormula(str);
		if(form != NULL)
		{
			cout<<form->ToString()<<endl;
		}
		else
		{
			cout<<"Syntax error."<<endl;
		}

		if(form != NULL && !form->IsAtomic()) delete form;
		

		cout<<"Type e to exit or press ENTER to continue...";
		if(cin.get() == 'e')
			break;
	}
	DestroyTable();

	return 0;
}