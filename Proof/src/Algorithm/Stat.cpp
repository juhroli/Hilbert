#include "Stat.h"

#include <sstream>

using std::stringstream;
using std::endl;

Stat::Stat()
	: m_startTime(0)
	, m_endTime(0)
	, m_elapsedTime(0.0)
	, m_startSize(0)
	, m_endSize(0)
	, m_MPCount(0)
	, m_axiomCount(0)
{
}

/*
*	Sets the starting time.
*/
void Stat::Stat_StartTimer()
{
	m_startTime = clock();
}

/*
*	Sets the ending time and calculates the elapsed time.
*/
void Stat::Stat_EndTimer()
{
	m_endTime = clock();

	m_elapsedTime = static_cast<double>(m_endTime - m_startTime) / CLOCKS_PER_SEC;
}

double Stat::Stat_GetElapsedTime()
{
	return m_elapsedTime;
}

void Stat::Stat_StartSize(int n)
{
	m_startSize = n;
}

void Stat::Stat_EndSize(int n)
{
	m_endSize = n;
}

/*
*	Returns the number of new formulas.
*/
int Stat::Stat_GetSizeDelta()
{
	return m_endSize - m_startSize;
}

void Stat::Stat_incMPCount()
{
	m_MPCount++;
}

void Stat::Stat_incAxiomCount()
{
	m_axiomCount++;
}

/*
*	Returns a string containing info of the statistics.
*/
string Stat::Stat_ToString()
{
	stringstream stream;

	stream << "Elapsed time: " << Stat_GetElapsedTime() << " s" <<endl
		<< "New formulas created: " << Stat_GetSizeDelta() << endl
		<< "Number of modus ponenses: " << m_MPCount << endl
		<< "Number of times axioms were used: " << m_axiomCount << endl;

	return stream.str();
}