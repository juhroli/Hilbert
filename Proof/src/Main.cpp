#include <iostream>
using namespace std;
#include "HilbertIncludes.h"
#include "Formula/AtomicFormula.h"

#include <string>
//#include <hash_set>
int main()
{
	IFormula * form = new AtomicFormula(1, "F");
	cout<<form->ToString()<<" "<<form->Eval()<<" "<<form->IsAtomic()<<endl;
	delete form;
	IFormula ** tomb;
	tomb = new IFormula * [3];
	tomb[0] = new AtomicFormula(1, "F");
	cout<<tomb[0]->ToString()<<" "<<tomb[0]->Eval()<<" "<<tomb[0]->IsAtomic()<<endl;
	tomb[1] = new AtomicFormula(2, "G");
	static_cast<AtomicFormula*>(tomb[1])->NegValue();
	cout<<tomb[1]->ToString()<<" "<<tomb[1]->Eval()<<" "<<tomb[1]->IsAtomic()<<endl;
	cout<<FALSE<<IMPLIES<<endl;
	/*hash_set<IFormula*> formulas;
	formulas.insert(form);
	*/
	delete tomb[0];
	delete tomb[1];
	delete[] tomb;
	system("PAUSE");
	return 0;
}