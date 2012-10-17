#pragma once

#include <memory>

/*
*	Delete macro for non-atomic formulas. Let the AtomicFormula table handle the Atomic Formulas' delete.
*/
#define DELETEFORMULA(x) if(x != __nullptr && !x->IsAtomic()) { delete x; } x = __nullptr;

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

const static char * IMPLIES = "->";
static char * FALSE = "~";