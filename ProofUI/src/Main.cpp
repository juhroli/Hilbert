#include <iostream>
#include <string>

#include "HilbertIncludes.h"
#include "Formula/Containers/AFormulaTable.h"
#include "Input/FormulaParser.h"
#include "Algorithm/General.h"
#include "Algorithm/AlgorithmBase.h"

using namespace AFormulaTable;
using namespace FormulaParser;
using namespace General;
using namespace std;

int main(int argc, char* argv[])
{
	/*
	*	Arguments:
	*	-fr [file]: read axioms and tasks from this file
	*	-fw [file]: write results to this file
	*	-alg [{0, 1, ..., n}]: algorithm type
	*	-algs: use this argument alone -> lists available algorithm types and descriptions
	*	-sigma ["formulas, ..."]: set of formulas
	*	-target ["formula"]: this is the target formula
	*	-axioms ["formulas, ..."]: set of formulas
	*/

	//TODO: argumentumok beolvasása
	//TODO: fájlokba lehessen konfigolni h milyen algot használjon?
	DestroyTable();

	return 0;
}