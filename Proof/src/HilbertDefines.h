#ifndef HilbertDefines_h
#define HilbertDefines_h

#include <memory>

#pragma warning (disable : 4267) //Ignore "conversion from 'size_t' to 'int/unsigned'" warnings (size_t is 64 bits when compiling for 64-bit)

/*
*	Delete macro for non-atomic formulas. Let the atomic formula table handle the atomic formulas' delete.
*	So this won't delete a formula if it's atomic...
*/
#define DELETEFORMULA(x) { if(x != nullptr && (!x->IsAtomic() || x->IsWrapped())) { delete x; } x = nullptr; }

/*
*	Extended delete with nullptr check.
*/
#define DELETE(x) if(x != nullptr) { delete x; x = nullptr; }

/*
*	Using the Cantor pairing function to create one integer from two.
*	It is a bijection:
*	f : N x N -> N (N := non-negative integers)
*	defined by
*	f(x, y) := ( (x + y) * (x + y + 1) ) / 2 + y
*
*	With this we can use an unordered map to identify
*	which formula pairs were used already then skip them.
*/
#define CPAIR(x, y) (((x + y) * (x + y + 1)) / 2 + y)

#define FALSE "~"
#define IMPLIES "->"

using std::shared_ptr;

class IFormula;
class AtomicFormula;
class TempFormula;
class ImplicationFormula;
class Axiom;

typedef shared_ptr<IFormula> spIFormula;
typedef shared_ptr<AtomicFormula> spAtomicFormula;
typedef shared_ptr<TempFormula> spTempFormula;
typedef shared_ptr<ImplicationFormula> spImplicationFormula;
typedef shared_ptr<Axiom> spAxiom;

#endif