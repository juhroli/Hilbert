#include <iostream>

#include "HilbertIncludes.h"
#include "Formula/Atomic/AtomicFormula.h"
#include "Formula/Atomic/TempFormula.h"
#include "Formula/Compound/ImplicationFormula.h"
#include "Formula/Compound/Axiom.h"
#include "Formula/Containers/AFormulaTable.h"
#include "Formula/Containers/HilbertAxioms.h"
#include "Input/FormulaParser.h"
#include "Formula/Containers/Sets/FormulaSetList.h"
#include "Formula/Containers/Sets/FormulaSetVector.h"
#include "Algorithm/General.h"
#include <string>
#include "Algorithm/Algorithm0.h"

using namespace AFormulaTable;
using namespace FormulaParser;
using namespace General;
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
	*	6: unification
	*	7 "Formula": alg0x00
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
		FormulaSetVector fset;
		for(int i = 1; i <= 100; i++)
		{
			stringstream stream;
			stream<<"(F"<<i<<" -> (G"<<i<<" -> H"<<i<<")) -> ((F"<<i<<" -> G"<<i<<") -> (F"<<i<<" -> H"<<i<<"))";
			fset.Add(ParseFormula(stream.str()));
		}
	}

	/*=================TEST FOR UNIFICATION=================*/
	if( argc > 1 && (atoi(argv[1]) == 6 || atoi(argv[1]) == 0))
	{
		IFormula * f1 = ParseFormula("(G->G)->(G->H)");
		IFormula * f2 = ParseTemp("F->(G->F)");
		IFormula * res = __nullptr;

		bool ret = Unification(f1, f2, res);
		
		cout<<(res != __nullptr && !res->IsNull() ? res->ToString() : "fail") <<endl;
		cout<<f1->ToString()<<endl<<f2->ToString()<<endl;
		DELETEFORMULA(res);
		DELETEFORMULA(f2);
		DELETEFORMULA(f1);
	}
	
	/*=================TEST FOR ALGORITHM 0x00=================*/
	/*
	*	EXAMPLES:
	*	P->((P->~)->~)
	*	F->F
	*	(G->H)->((F->G)->(F->H))
	*	~->F
	*	((P->~)->P)->((P->~)->~)
	*	((P->~)->~) -> ((P->Q) -> Q)
	*	P -> ((P->~)->Q)
	*	((P->(Q->R))->(P->Q))->((P->(Q->R))->(P->R))
	*	(P->Q) -> (P->(R->Q))
	*	(P->((Q->(R->Q))->S)) -> (P->S)
	*	(P->Q) -> ((R->P)->(R->Q))
	*	(P->~)->(P->Q)
	*	((P->~)->(Q->~)) -> (Q->P)
	*	((F->~)->(G->~))->(((F->~)->G)->F)
	*	((P->~)->P) -> P //Won't work
	*	
	*/
	if( argc > 1 && (atoi(argv[1]) == 7 || atoi(argv[1]) == 0))
	{
		IAlgorithm * alg = Create(ALG_0);
		IFormulaSet * fset = new FormulaSetList();
		IFormula * task = ParseFormula(argv[2]);
		cout<<"The task:"<<endl<<"|- "<<task->ToString()<<endl<<endl;
		alg->SetTask(fset, task);
		AxiomContainer * axioms = new HilbertAxioms();
		alg->SetAxioms(axioms);
		alg->SetMaxLength(14);
		static_cast<Algorithm0*>(alg)->SetSigmaLimit(500);
		alg->Start();
		cout<<alg->GetResult()<<endl;
		delete fset;
		delete axioms;
		delete alg;
	}

	
	DestroyTable();

	return 0;
}