#include "ImplicationFormula.h"

ImplicationFormula::ImplicationFormula()
	: m_left(__nullptr)
	, m_right(__nullptr)
	, m_length(0)
	, m_hash(0)
{
}

ImplicationFormula::ImplicationFormula(IFormula * left, IFormula * right)
	: m_left(left)
	, m_right(right)
{
	if(m_left != __nullptr && m_right != __nullptr)
	{
		this->GenerateHash();
		m_length = m_left->Length() + m_right->Length();
	}
	else
		m_length = 0;
}

ImplicationFormula::ImplicationFormula(ImplicationFormula& formula)
{
	m_left = formula.GetLeftSub();
	m_right = formula.GetRightSub();

	if(m_left != __nullptr && !m_left->IsAtomic())
		m_left = m_left->Clone();
	if(m_right != __nullptr && !m_right->IsAtomic())
		m_right = m_right->Clone();

	m_length = formula.Length();
	m_hash = formula.HashCode();
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
	return ( m_left->IsTemp() || m_right->IsTemp() );
}

bool ImplicationFormula::Eval()
{
	return ( !m_left || m_right );
}

bool ImplicationFormula::Equals(IFormula * formula)
{
	//TODO: check for temp formulas -> unification
	if(!formula || formula->IsAtomic())
		return false;

	return m_hash == formula->HashCode();
}

string ImplicationFormula::ToString()
{
	stringstream stream;

	if(!m_left->IsAtomic())
	{
		stream << "(";
		stream << m_left->ToString();
		stream << ")";
	}
	else
	{
		stream << m_left->ToString();
	}
	
	stream << IMPLIES;

	if(!m_right->IsAtomic())
	{
		stream << "(";
		stream << m_right->ToString();
		stream << ")";
	}
	else
	{
		stream << m_right->ToString();
	}

	return stream.str();
}

IFormula * ImplicationFormula::Clone()
{
	return new ImplicationFormula(*this);
}

bool ImplicationFormula::IsNull()
{
	return m_left == __nullptr || m_right == __nullptr;
}

IFormula * ImplicationFormula::Replace(IFormula& t, IFormula& x)
{
	if(t.IsTemp())
		return new ImplicationFormula(this->GetLeftSub()->Replace(t, x), this->GetRightSub()->Replace(t, x));
	return this;
}

unsigned ImplicationFormula::Length()
{
	return m_length;
}

long ImplicationFormula::HashCode()
{
	return m_hash;
}

void ImplicationFormula::SetLeftSub(IFormula * formula)
{
	m_left = formula;

	if(m_left != __nullptr && m_right != __nullptr)
	{
		GenerateHash();
		m_length = m_left->Length() + m_right->Length();
	}
	else
		m_length = 0;
}

void ImplicationFormula::SetRightSub(IFormula * formula)
{
	m_right = formula;

	if(m_left != __nullptr && m_right != __nullptr)
	{
		GenerateHash();
		m_length = m_left->Length() + m_right->Length();
	}
	else
		m_length = 0;
}

IFormula * ImplicationFormula::GetLeftSub()
{
	return m_left;
}

IFormula * ImplicationFormula::GetRightSub()
{
	return m_right;
}

string ImplicationFormula::ToStringPostfix()
{
	stringstream stream;

	if(m_left->IsAtomic())
		stream << m_left->ToString();
	else
		stream << static_cast<ImplicationFormula*>(m_left)->ToStringPostfix();

	if(m_right->IsAtomic())
		stream << m_right->ToString();
	else
		stream << static_cast<ImplicationFormula*>(m_right)->ToStringPostfix();
	stream << IMPLIES;

	return stream.str();
}

string ImplicationFormula::ToStringPrefix()
{
	stringstream stream;

	stream << IMPLIES;
	if(m_left->IsAtomic())
		stream << m_left->ToString();
	else
		stream << static_cast<ImplicationFormula*>(m_left)->ToStringPrefix();

	if(m_right->IsAtomic())
		stream << m_right->ToString();
	else
		stream << static_cast<ImplicationFormula*>(m_right)->ToStringPrefix();

	return stream.str();
}

void ImplicationFormula::GenerateHash()
{
	//Generate hash code
	string fs = this->ToString();
	locale loc;
	const collate<char>& coll = use_facet<collate<char>>(loc);
	m_hash = coll.hash(fs.data(), fs.end()._Ptr);
}