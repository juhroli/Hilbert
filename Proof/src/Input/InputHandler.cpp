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
	/*
	*	Checks if there's an implication sign starting from the iterator.
	*/
	bool IsImplies(string::iterator& it, string& str)
	{
		int len = strlen(IMPLIES);
		for(int i = 0; i < len; i++)
		{
			if((it + i) == str.end() || *(it + i) != IMPLIES[i])
				return false;
		}
		return true;
	}

	/*
	*	Checks if the iterator is pointing to an illegal character.
	*/
	bool IsIllegal(string::iterator& it, string& str)
	{
		if(it != str.end() && *it != FALSE[0] && !IsImplies(it, str) && *it != '_' && *it != '(' && *it != ')'
					&& !(*it >= 0 || *it <= 9) && !(*it >= 'A' || *it <= 'Z')
					&& !(*it >= 'a' || *it <= 'z') )
			return true;

		return false;
	}

	/*
	*	Converts a string to an implication formula.
	*/
	IFormula * StringToFormula(string str)
	{
		return StringToFormula(F_IMPLICATION, str);
	}

	/*
	*	Converts a string to a formula defined by a FormulaType.
	*/
	IFormula * StringToFormula(FormulaType type, string str)
	{
		//TODO: review + comments
		IFormula * ret = NULL;

		//Erase all spaces from the input
		string::size_type pos = 0;
		bool space = true;
		while(space)
		{
			pos = str.find(" ");
			if(pos != string::npos)
				str.erase(pos, 1);
			else
				space = false;
		}


		switch(type)
		{
		case F_FALSE:
			ret = AddAtomicFormula(new FalseFormula());
			break;
		case F_ATOMIC:
		case F_TEMP:
			{
			char * cStr = new char[str.length()];
			str.copy(cStr, str.length());

			if(type == F_TEMP)
			{
				ret = AddAtomicFormula(new TempFormula(cStr));
			}
			else
			{
				ret = AddAtomicFormula(new AtomicFormula(cStr));
			}
			}
			break;

		case F_AXIOM:
		case F_IMPLICATION:
			{
				//Check if the input string is shorter than the min length.
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
			
				//Iterate through the string.
				while(it != str.end())
				{
					//If it contains illegal characters break.
					if(IsIllegal(it, str))
						break;

					//If '(' is read push a new implication formula or axiom into the stack.
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

					//If IMPLIES is read pop a formula from the stack
					//and if it's not null then point to the top of the stack
					//(which should be an implication formula or an axiom)
					//then set the left sub formula to the first popped.
					if(IsImplies(it, str))
					{
						IFormula * top = formStack.top();
						formStack.pop();
						if(top->IsNull() || formStack.size() < 1)
							break;
						IFormula * impl = formStack.top();
						if(static_cast<ImplicationFormula*>(impl)->GetLeftSub() != NULL
							&& static_cast<ImplicationFormula*>(impl)->GetRightSub() == NULL)
						{
							//In this case a new implication formula without brackets is being read
							impl = new ImplicationFormula();
							formStack.push(impl);
						}
						static_cast<ImplicationFormula*>(impl)->SetLeftSub(top);
						it += strlen(IMPLIES);
						continue;
					}

					//In this case we read an atomic formula's string.
					if(*it != ')')
					{
						stream<<*it;
					}

					//If we read the end of the atomic formula's string,
					//create the atomic formula or get it from the table
					//and push it to the stack.
					if(*it != ')' && ((it + 1) == str.end() || *(it + 1) == IMPLIES[0] || *(it + 1) == ')'))
					{
						stream<<'\0';
						char * cStr = new char[stream.str().length()];
						stream.str().copy(cStr, stream.str().length());
						stream.str(""); //Empty the stream...

						AtomicFormula * atomic = NULL;
						if(type == F_IMPLICATION)
							atomic = GetAtomicFormula(cStr);
						else
							atomic = GetTempFormula(cStr);

						if(atomic == NULL)
						{
							if(cStr == FALSE)
							{
								atomic = new FalseFormula();
							}
							else
							{
								if(type == F_AXIOM)
									atomic = new TempFormula(cStr);
								else
									atomic = new AtomicFormula(cStr);
							}

							AddAtomicFormula(atomic);

						}

						formStack.push(atomic);
						if((it + 1) != str.end()) it++; //Iterate if the next pointer is not the end.
					}
					else
					{
						//Iterate if the currect pointer is not ')' and the next is not the end.
						if(*it != ')' && (it + 1) != str.end()) it++;
					}

					//If there's no more string for an atomic formula
					//and it is pointing to the end then
					//pop the formula from the top of the stack and
					//put it in the next top formula's right sub.
					if(stream.str().empty() && (*it == ')' || (it + 1) == str.end()))
					{
						IFormula * top = formStack.top();
						formStack.pop();
						if(top->IsNull() || formStack.size() < 1)
							break;
						IFormula * impl = formStack.top();
						if(static_cast<ImplicationFormula*>(impl)->GetLeftSub() == NULL)
							break;
						static_cast<ImplicationFormula*>(impl)->SetRightSub(top);


						if(formStack.size() > 2 && (!impl->IsNull())
							&& ( ((it + 1) != str.end() ? !IsImplies(it + 1, str) : true) && *it != ')'
							|| ((it + 1) != str.end() ? IsImplies(it + 1, str) : true) && *it == ')' )
							|| formStack.size() > 1 && !impl->IsNull() && (it + 1) == str.end())
						{
							top = impl;
							formStack.pop();
							impl = formStack.top();

							while(static_cast<ImplicationFormula*>(impl)->GetRightSub() == NULL)
							{
								if(static_cast<ImplicationFormula*>(impl)->GetLeftSub() == NULL)
								{
									formStack.push(top);
									break;
								}
								static_cast<ImplicationFormula*>(impl)->SetRightSub(top);
								if(formStack.size() == 1)
									break;
								top = formStack.top();
								if(formStack.size() > 2 || it == str.end() || (it + 1) == str.end())
									formStack.pop();

								impl = formStack.top();
								if(it != str.end() && !IsImplies(it, str) && (formStack.size() == 1 || (it + 1) != str.end() || *it != ')')) it++;
							}
						}

						//Iterate only if the stack's size is 1 or the next it is not the end
						//or the current it is a ')'. By this we can avoid to break when there
						//are still more formulas in the stack.
						if(it != str.end() && !IsImplies(it, str) && (formStack.size() == 1 || (it + 1) != str.end() || *it != ')')) it++;

						continue;
					}				
				}

				//By failure if the stack has more than 1 formulas clear them then return null.
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