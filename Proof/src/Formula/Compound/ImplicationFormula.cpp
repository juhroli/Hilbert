#include "../../HilbertDefines.h"
#include "ImplicationFormula.h"
#include "../Atomic/AtomicFormula.h"

ImplicationFormula::ImplicationFormula()
	: m_left(nullptr)
	, m_right(nullptr)
	, m_length(0)
	, m_hash(0)
	, m_string("")
	, m_hashString("")
	, m_temp(None)
{
}

ImplicationFormula::ImplicationFormula(IFormula * left, IFormula * right)
	: m_left(left)
	, m_right(right)
	, m_length(0)
	, m_hash(0)
	, m_string("")
	, m_hashString("")
	, m_temp(None)
{
}

ImplicationFormula::ImplicationFormula(ImplicationFormula& formula)
{
	m_left = formula.GetLeftSub();
	m_right = formula.GetRightSub();

	if(m_left != nullptr && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != nullptr && !m_right->IsAtomic())
		m_right = m_right->Clone();

	m_length = formula.m_length;
	m_hash = formula.m_hash;
	m_string = formula.m_string;
	m_hashString = formula.m_hashString;
	m_temp = formula.m_temp;
}

ImplicationFormula::~ImplicationFormula()
{
	DELETEFORMULA(m_left);
	DELETEFORMULA(m_right);
}

bool ImplicationFormula::IsAtomic()
{
	return false;
}

bool ImplicationFormula::IsTemp()
{
	if(m_temp == None && m_left != nullptr && m_right != nullptr)
	{
		m_temp = ( m_left->IsTemp() || m_right->IsTemp() ) ? True : False;
	}

	return m_temp == True;
}

bool ImplicationFormula::Eval()
{
	return ( !m_left->Eval() || m_right->Eval() );
}

bool ImplicationFormula::Equals(IFormula * formula)
{
	if(formula == nullptr || formula->IsAtomic())
		return false;

	return this->HashCode() == formula->HashCode();
}

string ImplicationFormula::ToString()
{
	if(m_string.empty() && m_left != nullptr && m_right != nullptr)
	{
		stringstream stream;
		stringstream hashStream; //It is needed, because temp's are stored like this: "_Symbol", so for them the GetSymbol() should be called

		if(!m_left->IsAtomic())
		{
			stringstream local;
			local << "(" << m_left->ToString() << ")";
			stream << local.str();
			hashStream << "(" << static_cast<ImplicationFormula*>(m_left)->GetHashString() << ")";
		}
		else
		{
			stream << m_left->ToString();
			hashStream << static_cast<AtomicFormula*>(m_left)->GetSymbol();
		}
	
		stream << IMPLIES;
		hashStream << IMPLIES;

		if(!m_right->IsAtomic())
		{
			stringstream local;
			local << "(" << m_right->ToString() << ")";
			stream << local.str();
			hashStream << "(" << static_cast<ImplicationFormula*>(m_right)->GetHashString() << ")";
		}
		else
		{
			stream << m_right->ToString();
			hashStream << static_cast<AtomicFormula*>(m_right)->GetSymbol();
		}

		m_string = stream.str();
		m_hashString = hashStream.str();
	}

	return this->m_string;
}

IFormula * ImplicationFormula::Clone()
{
	return new ImplicationFormula(*this);
}

IFormula * ImplicationFormula::Replace(IFormula * t, IFormula * x)
{
	if(t == nullptr || x == nullptr)
		return this->Clone();

	if(this->IsTemp() && t->IsTemp() && t->IsAtomic())
		return new ImplicationFormula(this->GetLeftSub()->Replace(t, x), this->GetRightSub()->Replace(t, x));
	return this->Clone();
}

unsigned ImplicationFormula::Length()
{
	if(m_length == 0 && m_left != nullptr && m_right != nullptr)
	{
		m_length = m_left->Length() + m_right->Length();
	}

	return m_length;
}

long ImplicationFormula::HashCode()
{
	if(m_hash == 0 && m_left != nullptr && m_right != nullptr)
	{
		if(m_hashString.empty())
			this->ToString();

		m_hash = GenerateHashCode(m_hashString);
	}

	return m_hash;
}

bool ImplicationFormula::IsWrapped()
{
	return false;
}

/*
*	Returns the string used for hashing.
*/
string ImplicationFormula::GetHashString()
{
	return m_hashString;
}

IFormula * ImplicationFormula::GetLeftSub()
{
	return m_left;
}

IFormula * ImplicationFormula::GetRightSub()
{
	return m_right;
}