#include "InputHandler.h"
#include "../Formula/TempFormula.h"
#include "../Formula/FalseFormula.h"
#include "../Formula/ImplicationFormula.h"
#include "../Formula/Axiom.h"
#include "../Formula/AFormulaTable.h"
#include <stack>

using std::stack;
using namespace AFormulaTable;

namespace InputHandler
{

	IFormula * StringToFormula(string str)
	{
		//By default create an implication formula
		return StringToFormula(str, F_IMPLICATION);
	}

	IFormula * StringToFormula(string str, FormulaType type)
	{
		//TODO: review + comments
		IFormula * ret = NULL;

		switch(type)
		{
		case F_ATOMIC:
		case F_TEMP:
		case F_FALSE:
			{
			char * cStr = new char[str.length()];
			str.copy(cStr, str.length());

			ret = AFormulaTable::GetAtomicFormula(cStr);
			if(ret == NULL)
			{
				if(type == F_FALSE)
				{
					ret = AFormulaTable::AddAtomicFormula(new FalseFormula());
				}
				else if(type == F_TEMP)
				{
					ret = AFormulaTable::AddAtomicFormula(new TempFormula(cStr));
				}
				else
				{
					ret = AFormulaTable::AddAtomicFormula(new AtomicFormula(cStr));
				}
			}
			}
			break;

		case F_AXIOM:
		case F_IMPLICATION:
			{
			if(str.length() < (strlen(IMPLIES) + 2))
			{
				break;
			}
			string::iterator it = str.begin();
			stack<IFormula*> formStack;
			stringstream stream;

			if(type == F_IMPLICATION)
			{
				formStack.push(new ImplicationFormula());
			}
			else
			{
				formStack.push(new Axiom());
			}
			
			while(it != str.end())
			{
				if(*it == ' ')
				{
					if(it != str.begin() && *(it - 1) != ' ' && *(it - 1) != '(' && *(it - 1) != '>' && *(it - 1) != ')'
						&& *(it + 1) != ' ' && *(it + 1) != '(' && *(it + 1) != '-' && *(it + 1) != ')')
						break;
					it++;
					continue;
				}
				if(*it == '(')
				{
					if(type == F_IMPLICATION)
					{
						formStack.push(new ImplicationFormula());
					}
					else
					{
						formStack.push(new Axiom());
					}
					it++;
					continue;
				}
				if(*it == '-' && *(it + 1) == '>')
				{
					IFormula * top = formStack.top();
					formStack.pop();
					if(top->IsNull())
						break;
					IFormula * impl = formStack.top();
					static_cast<ImplicationFormula*>(impl)->SetLeftSub(top);
					it += 2;
					continue;
				}

				//In this case we read an atomic formula
				if(*it != ')')
				{
					stream<<*it;
				}

				if(*it != ')' && ((it + 1) == str.end() || *(it + 1) == ' ' || *(it + 1) == '-' || *(it + 1) == ')'))
				{
					stream<<'\0';
					char * cStr = new char[stream.str().length()];
					stream.str().copy(cStr, stream.str().length());
					stream.str(""); //Empty the stream...
					AtomicFormula * atomic;
					atomic = AFormulaTable::GetAtomicFormula(cStr);
					if(atomic == NULL)
					{
						if(cStr == FALSE)
						{
							atomic = new FalseFormula();
						}
						else
						{
							if(type == F_AXIOM)
							{
								atomic = new TempFormula(cStr);
							}
							else
							{
								atomic = new AtomicFormula(cStr);
							}
						}

						AFormulaTable::AddAtomicFormula(atomic);

					}

					formStack.push(atomic);
					if((it + 1) != str.end()) it++;
				}
				else
				{
					if(*it != ')' && (it + 1) != str.end()) it++;
				}

				if(stream.str().empty() && (*it == ')' || (it + 1) == str.end()))
				{
					IFormula * top = formStack.top();
					formStack.pop();
					if(top->IsNull())
						break;
					IFormula * impl = formStack.top();
					if(static_cast<ImplicationFormula*>(impl)->GetLeftSub() == NULL)
						break;
					static_cast<ImplicationFormula*>(impl)->SetRightSub(top);
					if(formStack.size() == 1 || (it + 1) != str.end() || *it != ')') it++;
					continue;
				}
				if(*it == ')' && *(it - 1) != ' ') it++;
				
			}
			if(formStack.size() > 1)
			{
				while(!formStack.empty())
					formStack.pop();
				return NULL;
			}
			if(!formStack.empty())
			{
				ret = formStack.top()->Clone();
				formStack.pop();
				if(ret->IsNull())
				{
					return NULL;
				}
			}

			}
			break;
		}
		return ret;
	}
}