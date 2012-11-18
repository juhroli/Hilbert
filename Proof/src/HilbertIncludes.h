#ifndef HilbertIncludes_h
#define HilbertIncludes_h

/*
          ^^         |         ^^
          ::         |         ::
   ^^     ::         |         ::     ^^
   ::     ::         |         ::     ::
    ::     ::        |        ::     ::
      ::    ::       |       ::    ::
        ::    ::   _/~\_   ::    ::
          ::   :::/     \:::   ::
            :::::(       ):::::
                  \ ___ /
             :::::/`   `\:::::
           ::    ::\o o/::    ::
         ::     ::  :":  ::     ::
       ::      ::   ` `   ::      ::
      ::      ::           ::      ::
     ::      ::             ::      ::
     ^^      ::             ::      ^^
             ::             ::
             ^^             ^^
	This spider protects the code from bugs.
	Include this file in every other source.
*/
#include <memory>

/*
*	Delete macro for non-atomic formulas. Let the atomic formula table handle the atomic formulas' delete.
*	So this won't delete a formula if it's atomic...
*/
#define DELETEFORMULA(x) if(x != nullptr && !x->IsAtomic()) { delete x; } x = nullptr;

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