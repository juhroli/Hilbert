Efficient implementation of Hilbert's system

Simply the main goal is to create a library that can give the steps of the proof of a given Sigma |- F task. For this we can use different algorithms. There are four algorithms to choose from.

This project is made in Visual Studio 2012.

Linux support is added.
Tested with gcc 4.6.3

Binaries: https://www.dropbox.com/sh/rbx9hzyhmgo0gbp/XzXTENXlw7

How to compile (with static library):

	Use the makefile in Proof/src
	
	OR

	First cd to Proof/src then use this command to create the objects:
	g++ -std=c++0x -c Formula/Atomic/*.cpp Formula/Compound/*.cpp Formula/Containers/*.cpp Formula/Containers/Sets/*.cpp Input/*.cpp Algorithm/*.cpp

	Then to create the library:
	ar rcs libproof.a *.o

	
	Linking is simple, for example:
	g++ -std=c++0x Main.cpp -L. -lproof -o ProofUI
	But the library's headers should be availabe for the code, which means they should be in the same directory.


Some examples of proofs made by the lib using the axioms:
1. (a -> (b -> c)) -> ((a -> b) -> (a -> c))
2. a -> (b -> a)
3. ((a -> ~) -> ~) -> a

The task:
|- ((P->~)->P)->P

After applying deduction: 
{ (P->~)->P } |- P


The proof is:
1. (P->~)->P    from Sigma

2. (P->~)->(P->(P->~))    Axiom: a->(b->a) [a/P->~] [b/P] 

3. ((P->~)->(P->(P->~)))->(((P->~)->P)->((P->~)->(P->~)))    Axiom: (a->(b->c))->((a->b)->(a->c)) [a/P->~] [c/P->~] [b/P] 

4. ((P->~)->P)->((P->~)->(P->~))    : Cut ((P->~)->(P->(P->~)))->(((P->~)->P)->((P->~)->(P->~))) with (P->~)->(P->(P->~))

5. (P->~)->(P->~)    : Cut ((P->~)->P)->((P->~)->(P->~)) with (P->~)->P

6. ((P->~)->(P->~))->(((P->~)->P)->((P->~)->~))    Axiom: (a->(b->c))->((a->b)->(a->c)) [a/P->~] [b/P] [c/~] 

7. ((P->~)->P)->((P->~)->~)    : Cut ((P->~)->(P->~))->(((P->~)->P)->((P->~)->~)) with (P->~)->(P->~)

8. (P->~)->~    : Cut ((P->~)->P)->((P->~)->~) with (P->~)->P

9. ((P->~)->~)->P    Axiom: ((a->~)->~)->a [a/P] 

10. P    : Cut ((P->~)->~)->P with (P->~)->~


The task:
|- ~->F

After applying deduction: 
{ ~ } |- F


The proof is:
1. ~    from Sigma

2. ~->((F->~)->~)    Axiom: a->(b->a) [a/~] [b/F->~] 

3. (F->~)->~    : Cut ~->((F->~)->~) with ~

4. ((F->~)->~)->F    Axiom: ((a->~)->~)->a [a/F] 

5. F    : Cut ((F->~)->~)->F with (F->~)->~

The task:
|- ((P->(Q->R))->(P->Q))->((P->(Q->R))->(P->R))

After applying deduction: 
{ (P->(Q->R))->(P->Q), P->(Q->R), P } |- R


The proof is:
1. P    from Sigma

2. P->(Q->R)    from Sigma

3. (P->(Q->R))->(P->Q)    from Sigma

4. P->Q    : Cut (P->(Q->R))->(P->Q) with P->(Q->R)

5. Q    : Cut P->Q with P

6. Q->R    : Cut P->(Q->R) with P

7. R    : Cut Q->R with Q