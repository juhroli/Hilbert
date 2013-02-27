#include <iostream>
#include <string>
#include <sstream>

#ifdef _MSC_VER
	#include "../../Proof/libheader/ProofLibHeader.h"
#else
	#include "ProofLibHeader.h"
#endif

using namespace AFormulaTable;
using namespace FormulaParser;
using namespace General;
using std::cout;
using std::endl;
using std::stringstream;

//#define H_TEST
#ifndef H_TEST
int main(int argc, char* argv[])
{
	/*
	*	Arguments:
	*	-fr [file]: read axioms and task from this file
	*	-alg [algorithm type] [max sigma size] [max formula length] [extra parameter]: the last three are optional
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
	int maxFormulaLength = 0;
	int maxSigmaSize = 0;
	unsigned extraParm = UINT32_MAX;
	bool fromFile = false;
	bool help = false;
	bool showStat = false;

	try
	{
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

				if(argc > (i + 1) && argv[i + 1][0] != '-')
					stringstream(argv[++i]) >> extraParm;

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
					throw "Error 2: Failed to parse target formula.";

				continue;
			}

			//Axioms
			if(!fromFile && strcmp(argv[i], "-axioms") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
			{
				if( !(axioms = ParseAxioms(argv[++i])) )
					throw "Error 3: Failed to parse axioms.";

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
					<< "-alg [algorithm type] [max sigma size] [max formula length] [extra parameter]: the last three are optional; 0 will use their default values" << endl
					<< "-algs: lists available algorithm types and descriptions" << endl
					<< "-sigma [\"{ formula, ... }\"]: set of formulas" << endl
					<< "-target [\"formula\"]: this is the target formula" << endl
					<< "-axioms [\"{ formula, ... }\"]: set of formulas" << endl
					<< "-stat: show statistics" << endl
					<< "-help: obvious" << endl;
				break;
			}

			throw "Error 7: Invalid argument(s).";
		}

		if(!help)
		{
			bool algFail;

			if(extraParm != UINT32_MAX)
				algFail = !(algorithm = CreateAlgorithm(algType, extraParm));
			else
				algFail = !(algorithm = CreateAlgorithm(algType));

			if(algFail)
				throw "Error 1: Failed to create algorithm instance.";

			if(maxSigmaSize > 0)
				algorithm->SetSigmaLimit(maxSigmaSize);

			if(maxFormulaLength > 0)
				algorithm->SetMaxLength(maxFormulaLength);

			if(fromFile)
			{
				if( !(algorithm->ReadFromFile(file)) )
					throw "Error 4: Failed to read from file.";
			}
			else
			{
				//Check if axioms were set, if not use Hilbert axioms
				if(axioms == nullptr)
					axioms = new HilbertAxioms();

				algorithm->SetAxioms(axioms);

				if(!sigmaStr.empty() && !(sigma = ParseFormulaSet(sigmaStr, algorithm->GetFSetType())) )
					throw "Error 5: Failed to parse formula set.";

				if(target != nullptr)
					algorithm->SetTask(sigma, target);
				else
					throw "Error 6: No target formula.";
			}

			algorithm->Start();
		
			cout << algorithm->GetResult() << endl;

			if(showStat && algorithm->IsFinished())
				cout <<endl << algorithm->Stat_ToString() << endl;
		}
	}
	catch(char * error)
	{
		cout << error << endl;
	}
	catch(char const * error)
	{
		cout << error << endl;
	}

	DELETE(sigma);
	DELETEFORMULA(target);
	DELETE(axioms);
	DELETE(algorithm);

	DestroyTable();

	return 0;
}
#else
int main(int argc, char* argv[])
{
	
	if(argc == 1)
	{
		cout << "No arguments. Use -help for more info." << endl;
		return 0;
	}

	AlgorithmBase * algorithm = nullptr;
	AxiomContainer * axioms = nullptr;
	IFormula * target = nullptr;
	IFormulaSet * sigma = nullptr;
	AlgorithmType algType = ALG_0x03;
	string file = "";
	string sigmaStr = "";
	int maxFormulaLength = 0;
	int maxSigmaSize = 0;
	unsigned seed = UINT32_MAX;
	unsigned knownMin = 0;
	bool fromFile = false;
	bool help = false;
	bool showStat = false;

	try
	{
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
				if(argc > (i + 1) && argv[i + 1][0] != '-')
					maxSigmaSize = atoi(argv[++i]);

				if(argc > (i + 1) && argv[i + 1][0] != '-')
					maxFormulaLength = atoi(argv[++i]);

				if(argc > (i + 1) && argv[i + 1][0] != '-')
					stringstream(argv[++i]) >> seed;

				if(argc > (i + 1) && argv[i + 1][0] != '-')
					knownMin = atoi(argv[++i]);

				continue;
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
					throw "Error 2: Failed to parse target formula.";

				continue;
			}

			//Axioms
			if(!fromFile && strcmp(argv[i], "-axioms") == 0 && argc > (i + 1) && argv[i + 1][0] != '-')
			{
				if( !(axioms = ParseAxioms(argv[++i])) )
					throw "Error 3: Failed to parse axioms.";

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
					<< "-alg [max sigma size] [max formula length] [seed] [known min]: seed is the random seed, min method {0, 1} -> 0: search by min sigma size, 1: by MP count, will stop the search at known min" << endl
					<< "-sigma [\"{ formula, ... }\"]: set of formulas" << endl
					<< "-target [\"formula\"]: this is the target formula" << endl
					<< "-axioms [\"{ formula, ... }\"]: set of formulas" << endl
					<< "-stat: show statistics" << endl
					<< "-help: obvious" << endl;
				break;
			}

			throw "Error 7: Invalid argument(s).";
		}

		if(!help)
		{
			unsigned lastGoodSeed = seed;
			unsigned last = UINT32_MAX;
			unsigned lastSize = maxSigmaSize;

			for(; seed < UINT32_MAX; seed++)
			{
				bool algFail;

				algFail = !(algorithm = CreateAlgorithm(algType, seed));

				if(algFail)
					throw "Error 1: Failed to create algorithm instance.";

				algorithm->SetSigmaLimit(lastSize);

				if(maxFormulaLength > 0)
					algorithm->SetMaxLength(maxFormulaLength);

				if(fromFile)
				{
					if( !(algorithm->ReadFromFile(file)) )
						throw "Error 4: Failed to read from file.";
				}
				else
				{
					//Check if axioms were set, if not use Hilbert axioms
					if(axioms == nullptr)
						axioms = new HilbertAxioms();

					algorithm->SetAxioms(axioms);

					if(!sigmaStr.empty() && !(sigma = ParseFormulaSet(sigmaStr, algorithm->GetFSetType())) )
						throw "Error 5: Failed to parse formula set.";

					if(target != nullptr)
						algorithm->SetTask(sigma, target);
					else
						throw "Error 6: No target formula.";
				}

				algorithm->Start();
		
				algorithm->GetResult();

				if(last > algorithm->Stat_MPCount() && algorithm->IsFinished())
				{
					last = algorithm->Stat_MPCount();
					lastSize = lastSize > algorithm->Stat_EndSize() ? algorithm->Stat_EndSize() : lastSize;
					lastGoodSeed = seed;

					cout << "Seed: " << seed << endl << "Min MP: " << last << endl;
					if(showStat)
						cout <<endl << algorithm->Stat_ToString() << endl;
				}

				if(seed % 1000 == 0)
					cout << seed << endl;

				if(last == knownMin)
					break;

				DELETE(axioms);
				DELETE(algorithm);
			}
		}
	}
	catch(char * error)
	{
		cout << error << endl;
	}
	catch(char const * error)
	{
		cout << error << endl;
	}

	DELETE(sigma);
	DELETEFORMULA(target);
	DELETE(axioms);
	DELETE(algorithm);

	DestroyTable();

	return 0;
}
#endif