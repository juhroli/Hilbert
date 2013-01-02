#include "FormulaGenerator.h"

#include "Formula/Containers/AFormulaTable.h"
#include "Formula/Compound/ImplicationFormula.h"
#include "Formula/Containers/Sets/FormulaSetList.h"
#include <ctime>
#include <deque>

using namespace AFormulaTable;
using std::deque;
using std::endl;

FormulaGenerator::FormulaGenerator()
	: m_N(0)
	, m_mask(3)
{
	srand((unsigned)time(0));
	SetDefaults();
}

FormulaGenerator::FormulaGenerator(unsigned n)
	: m_N(n)
	, m_mask(3)
{
	srand((unsigned)time(0));
	SetDefaults();
}

FormulaGenerator::~FormulaGenerator()
{
	DestroyTable();
}

/*
*	The main function of the generator.
*	First, generate a start set of formulas	with a max variable number of m_N.
*	Then randomly select a formula from the set and generate its contradiction c,
*	erase every other formula from the set which contradicts with c. The new clauses
*	are put in a vector which in the end will give an unsat CNF. Then some clauses
*	are randomly put in another set, which will be the right side of the consequence.
*	In the end, every bit vector of formulas are converted into AtomicFormulas and
*	ImplicationFormulas.
*/
string FormulaGenerator::Generate()
{
	formulas startSet;
	formulas clauses; //Conjunction of clauses
	formulas rightSide; //Disjunction of formulas

	GenStartSet(1, startSet, m_N);
	GenStartSet(2, startSet, m_N);

	while(startSet.size() > 0)
	{
		formula v = startSet[rand() % startSet.size()];
		formula c = GenContradiction(v);

		clauses.push_back(c);

		for(int i = 0; i < startSet.size(); i++)
		{
			//Check for contradictions with c
			if( (c & startSet[i]) == 0 )
			{
				startSet.erase(startSet.begin() + i);
				i--;
			}
		}
	}
	
	//Drop the same clauses
	for(unsigned i = 0; i < clauses.size(); i++)
	{
		formula it = clauses[i];

		for(int j = i + 1; j < clauses.size(); j++)
		{
			if(it == clauses[j])
			{
				clauses.erase(clauses.begin() + j);
				j--;
			}
		}
	}

	//Randomly select some formulas for the right side
	unsigned rightN = rand() % (clauses.size() / 5 + 1) + 1;

	for(unsigned i = 0; i < rightN; i++)
	{
		unsigned pos = rand() % clauses.size();
		rightSide.push_back(Negate(clauses[pos]));
		clauses.erase(clauses.begin() + pos);
	}

	/*
	*	Formulas in rightSide now have conjunctive connective and rightSide has disjunctive conn.
	*/

	FormulaSetList fListRight;

	//Convert the bits to Proof lib's IFormulas and add them to a set.
	for(unsigned i = 0; i < rightSide.size(); i++)
	{
		fListRight.Add(ConvertToFormula(rightSide[i], false));
	}

	//Disjunct those formulas in the set
	IFormula * right = DisjunctFormulas(fListRight);

	FormulaSetList fListLeft;

	for(unsigned i = 0; i < clauses.size(); i++)
	{
		fListLeft.Add(ConvertToFormula(clauses[i], true));
	}

	stringstream stream;

	stream << "default" << endl << right->ToString();
	if(fListLeft.Size() > 0)
		stream << endl << fListLeft.ToString();

	DELETEFORMULA(right);

	return stream.str();
}

/*
*	Generating the startig set of formulas into
*	the input set.
*	Creates (2^n)-1 formulas.
*/
void FormulaGenerator::GenStartSet(formula v, formulas& set, int n)
{
	set.push_back(v);
	n -= 1;

	if(n == 0)
		return;

	formula shift = v << 2;

	GenStartSet(shift + 1, set, n); //True branch
	GenStartSet(shift + 2, set, n); //False branch
}

/*
*	Generates a contradiction to the input v
*	v's contradiction is created by randomly negating
*	a variable or leaving it out of the formula by setting
*	its bits to 00. Also before setting a variable to 00,
*	we check if v is not a power of two, because if it were,
*	the whole v would be set to 0.
*/
formula FormulaGenerator::GenContradiction(formula v)
{
	for(unsigned i = 0; i < m_N; i++)
	{
		unsigned p = rand() % 2;
		
		if(p && !(v && !(v & (v - 1))))
		{
			v &= ~(m_mask << (2 * i)); //Set the atomic to 00
		}
		else
		{
			v ^= m_mask << (2 * i); //Negate the atomic
		}
	}

	Normalize(v);

	return v;
}

/*
*	Normalize v: turn 11 bits into 00.
*/
void FormulaGenerator::Normalize(formula& v)
{
	for(unsigned i = 0; i < m_N; i++)
	{
		if((v & (m_mask << (2 * i))) == m_mask << (2 * i))
			v &= ~(m_mask << (2 * i)); //Set it to 00.
	}
}

/*
*	Returns the negated formula of v.
*/
formula FormulaGenerator::Negate(formula v)
{
	for(unsigned i = 0; i < m_N; i++)
	{
		v ^= m_mask << (2 * i);
	}

	Normalize(v);

	return v;
}

/*
*	Disjuncts the formulas in the set.
*/
IFormula * FormulaGenerator::DisjunctFormulas(FormulaSetList flist)
{
	IFormula * ret = nullptr;
	for(auto it = flist.Begin(); it != flist.End(); it++)
	{
		if(!ret)
			ret = it->get()->Clone();
		else
		{
			if(!it->get()->Eval())
				ret = new ImplicationFormula(static_cast<ImplicationFormula*>(it->get())->GetLeftSub()->Clone(), ret);
			else if(!ret->Eval())
			{
				IFormula * prevRet = ret;

				ret = new ImplicationFormula(static_cast<ImplicationFormula*>(ret)->GetLeftSub()->Clone(), it->get()->Clone());

				DELETEFORMULA(prevRet);
			}
			else
			{
				unsigned subPos = rand() % 2;

				if(subPos)
					ret = new ImplicationFormula(new ImplicationFormula(ret, AddAtomicFormula(FALSE).get()), it->get()->Clone());
				else
					ret = new ImplicationFormula(new ImplicationFormula(it->get()->Clone(), AddAtomicFormula(FALSE).get()), ret);
			}
		}
	}

	return ret;
}

/*
*	Convert a formulas instance into IFormula*
*	If disjConn is true, then v has disjunctive connective, else conjunctive.
*/
IFormula * FormulaGenerator::ConvertToFormula(formula v, bool disjConn)
{
	int posN = 0; //number of true formulas
	int negN = 0; //number of false formulas

	deque<IFormula*> result; //Positive formulas will start from the front, negatives will be added after them

	IFormula * F = nullptr;
	IFormula * ret = nullptr;

	//Create the individual atomic formulas
	for(unsigned i = 0; i < m_N; i++)
	{
		formula u;

		u = v & (m_mask << (2 * i));
		u >>= i * 2;

		//The atomic formula is true
		if(u & 1)
		{
			stringstream stream;
			stream << (char)('A' + i) << '\0';
			F = AddAtomicFormula((char*)stream.str().data()).get();
			posN++;
			result.push_front(F);
		}

		//The atomic formula is false
		if(u & 2)
		{
			// A -> ~
			stringstream stream;
			stream << (char)('A' + i) << '\0';
			F = new ImplicationFormula(AddAtomicFormula((char*)stream.str().data()).get(), AddAtomicFormula(FALSE).get());
			negN++;
			result.push_back(F);
		}
	}

	/*
	*	Create the implication formula from the atomic formulas.
	*	The formulas are selected by their Eval() value to
	*	try to shorten the result's length.
	*/
	while(result.size() > 1)
	{
		IFormula * G = nullptr;
		IFormula * H = nullptr;

		if(posN + negN > 0)
		{
			unsigned pos = rand() % (posN + negN);
			G = result[pos]; //Choose a random atomic
			result.erase(result.begin() + pos); //Drop G from the deque

			//If G is true
			if(G->Eval())
			{
				posN--;
				if(negN > 0)
				{
					pos = (rand() % negN) + posN;
					negN--;
				}
				else
				{
					pos = rand() % result.size();
					posN--;
				}

				H = result[pos];

				result.erase(result.begin() + pos);

				if(disjConn)
				{
					F = new ImplicationFormula(
						H->Eval() ? new ImplicationFormula(H, AddAtomicFormula(FALSE).get())
						: static_cast<ImplicationFormula*>(H)->GetLeftSub(), G);
				}
				else
				{
					F = new ImplicationFormula(
							new ImplicationFormula(G,
								H->Eval() ? new ImplicationFormula(H, AddAtomicFormula(FALSE).get())
									: static_cast<ImplicationFormula*>(H)->GetLeftSub()),
							AddAtomicFormula(FALSE).get());
				}
			}
			else
			{
				negN--;
				if(posN > 0)
				{
					pos = (rand() % posN);
					posN--;
				}
				else
				{
					pos = rand() % result.size();
					negN--;
				}

				H = result[pos];

				result.erase(result.begin() + pos);
				
				if(disjConn)
				{
					F = new ImplicationFormula(static_cast<ImplicationFormula*>(G)->GetLeftSub(), H);
				}
				else
				{
					//(H->G)->~
					F = new ImplicationFormula(
							new ImplicationFormula(H,
								static_cast<ImplicationFormula*>(G)->GetLeftSub()),
							AddAtomicFormula(FALSE).get());
				}
			}
		}
		else
		{
			IFormula * G = nullptr;
			IFormula * H = nullptr;

			unsigned posG = rand() % result.size();

			G = result[posG];

			result.erase(result.begin() + posG);

			unsigned posH = rand() % result.size();

			H = result[posH];

			result.erase(result.begin() + posH);

			unsigned subPos = rand() % 2; //The position of the sub formulas

			if(subPos)
			{
				IFormula * X = H;
				H = G;
				G = X;
			}

			if(disjConn)
			{
				//F := (G -> ~) -> H
				F = new ImplicationFormula(new ImplicationFormula(G, AddAtomicFormula(FALSE).get()), H);
			}
			else
			{
				//F := ((G->(H->~))->~
				F = new ImplicationFormula(
						new ImplicationFormula(G,
							new ImplicationFormula(H,
							AddAtomicFormula(FALSE).get())),
						AddAtomicFormula(FALSE).get());
			}
		}

		result.push_back(F);
	}

	ret = result[0];

	return ret;
}