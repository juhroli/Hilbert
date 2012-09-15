#include <iostream>

#include "HilbertIncludes.h"
#include "Formula/AtomicFormula.h"
#include "Formula/TempFormula.h"
#include "Formula/ImplicationFormula.h"
#include "Formula/Axiom.h"
#include "Formula/AFormulaTable.h"
#include "Formula/HilbertAxioms.h"
#include "Input/InputHandler.h"

#include <string>

using namespace InputHandler;
using namespace AFormulaTable;
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
	string str;
	while(true)
	{
		cout<<"Write an implication formula: "<<endl;
		cin.clear();
		str.clear();
		getline(cin, str);
		IFormula * form = StringToFormula(str);
		if(form != NULL)
		{
			cout<<form->ToString()<<endl;
		}
		else
		{
			cout<<"Syntax error."<<endl;
		}

		if(form != NULL) delete form;

		if(cin.get() == 'e')
			break;
	}

	DestroyTable();
	return 0;
}