#include <iostream>
#include <string>

#include "HilbertIncludes.h"
#include "Formula/Containers/AFormulaTable.h"
#include "Input/FormulaParser.h"
#include "Algorithm/General.h"
#include "Algorithm/AlgorithmBase.h"
#include "Formula/Containers/HilbertAxioms.h"

using namespace AFormulaTable;
using namespace FormulaParser;
using namespace General;
using namespace std;

int main(int argc, char* argv[])
{
	/*
	*	Arguments:
	*	-fr [file]: read axioms and task from this file
	*	//-fw [file]: write results to this file
	*	-alg [algorithm type] [max sigma size] [max formula length]: the last two are optional
	*	-algs: use this argument alone -> lists available algorithm types and descriptions
	*	-sigma ["{ formula, ... }"]: set of formulas
	*	-target ["formula"]: this is the target formula
	*	-axioms ["{ formula, ... }"]: set of formulas
	*	-stat: show statistics
	*	-help: obvious
	*/

	if(argc == 1)
	{
		cout << "No arguments. Use -help for more info." << endl;
		return 0;
	}

	AlgorithmBase * algorithm = nullptr;
	AxiomContainer * axioms = nullptr;
	IFormula * target = nullptr;
	IFormulaSet * sigma = nullptr;
	AlgorithmType algType = ALG_0x00;
	string file = "";
	string sigmaStr = "";
	string errorCode = "";
	int maxFormulaLength = 0;
	int maxSigmaSize = 0;
	bool fromFile = false;
	bool help = false;
	bool showStat = false;

	/*================= Read arguments =================*/
	for(int i = 1; i < argc; i++)
	{
		//Read from file
		if(strcmp(argv[i], "-fr") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
		{
			file = argv[++i];
			fromFile = true;
			continue;
		}

		//Algorithm type
		if(strcmp(argv[i], "-alg") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
		{
			algType = (AlgorithmType)atoi(argv[++i]);

			if(argc > (i + 1) && argv[i + 1][0] != '-')
				maxSigmaSize = atoi(argv[++i]);

			if(argc > (i + 1) && argv[i + 1][0] != '-')
				maxFormulaLength = atoi(argv[++i]);

			continue;
		}

		//List algorithms
		if(strcmp(argv[i], "-algs") == 0)
		{
			help = true;
			cout << AlgorithmsDescription() << endl;
			break;
		}

		//Sigma
		if(!fromFile && strcmp(argv[i], "-sigma") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
		{
			sigmaStr = argv[++i];
			continue;
		}

		//Target
		if(!fromFile && strcmp(argv[i], "-target") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
		{
			if( !(target = ParseFormula(argv[++i])) )
			{
				errorCode = "Error 2: Failed to parse target formula.";
				goto error;
			}
			continue;
		}

		//Axioms
		if(!fromFile && strcmp(argv[i], "-axioms") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
		{
			if( !(axioms = ParseAxioms(argv[++i])) )
			{
				errorCode = "Error 3: Failed to parse axioms.";
				goto error;
			}
			continue;
		}

		//Stat
		if(strcmp(argv[i], "-stat") == 0)
		{
			showStat = true;
			continue;
		}

		//Help
		if(strcmp(argv[i], "-help") == 0)
		{
			help = true;

			cout
				<< "Arguments:" << endl
				<< "-fr [file]: read axioms and task from this file" << endl
				<< "\tA file looks like this:" << endl
				<< "\t1. line: \"{ axiom_1, ..., axiom_n }\" OR \"default\" //default means to use Hilbert axioms" << endl
				<< "\t2. line: \"formula\" //this is the target formula" << endl
				<< "\t3. line: \"{ formula_1, ..., formula_n }\" //OR leave this line empty; this conatains the formula set" << endl
				<< "-alg [algorithm type] [max sigma size] [max formula length]: the last two are optional; 0 will use their default values" << endl
				<< "-algs: use this argument alone -> lists available algorithm types and descriptions" << endl
				<< "-sigma [\"{ formula, ... }\"]: set of formulas" << endl
				<< "-target [\"formula\"]: this is the target formula" << endl
				<< "-axioms [\"{ formula, ... }\"]: set of formulas" << endl
				<< "-stat: show statistics" << endl
				<< "-help: obvious" << endl;
			break;
		}
	}

	if(!help)
	{
		//Check if axioms were set, if not use Hilbert axioms
		if(!axioms)
			axioms = new HilbertAxioms();

		if( !(algorithm = CreateAlgorithm(algType)) )
		{
			errorCode = "Error 1: Failed to create algorithm instance.";
			goto error;
		}

		if(maxSigmaSize > 0)
			algorithm->SetSigmaLimit(maxSigmaSize);

		if(maxFormulaLength > 0)
			algorithm->SetMaxLength(maxFormulaLength);

		if(fromFile)
		{
			if( !(algorithm->ReadFromFile(file)) )
			{
				errorCode = "Error 4: Failed to read from file.";
				goto error;
			}
		}
		else
		{
			algorithm->SetAxioms(axioms);

			if(!sigmaStr.empty() && !(sigma = ParseFormulaSet(sigmaStr, algorithm->GetFSetType())) )
			{
				errorCode = "Error 5: Failed to parse formula set.";
				goto error;
			}

			algorithm->SetTask(sigma, target);
		}

		algorithm->Start();
		
		cout << algorithm->GetResult() << endl;

		if(showStat && algorithm->IsFinished())
			cout <<endl << algorithm->Stat_ToString() << endl;
	}
	
error:
	if(!errorCode.empty())
		cout << errorCode << endl;

	DELETE(sigma);
	DELETEFORMULA(target);
	DELETE(axioms);
	DELETE(algorithm);

	DestroyTable();

	return 0;
}