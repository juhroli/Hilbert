#ifndef Stat_H
#define Stat_H

#include "../HilbertDefines.h"

#include <ctime>
#include <string>

using std::string;

/*
*	Statistic class for algorithms.
*	Stores the elapsed time in seconds and the number of modus ponenses,
*	new formulas and axiom usages.
*/
class Stat
{
public:
	Stat();
	~Stat() {}

	double Stat_GetElapsedTime();
	int Stat_GetSizeDelta();
	unsigned Stat_MPCount();
	unsigned Stat_EndSize();

	string Stat_ToString();

private:
	clock_t m_startTime;
	clock_t m_endTime;
	double m_elapsedTime;
	int m_startSize; //number of formulas in set in the beginning
	int m_endSize; //number of formulas in set in the end
	unsigned m_MPCount; //number of modus ponenses
	unsigned m_axiomCount; //number of times axioms were used

protected:
	void Stat_StartTimer();
	void Stat_EndTimer();
	void Stat_StartSize(int n);
	void Stat_EndSize(int n);

	void Stat_incMPCount();
	void Stat_incAxiomCount();
};


#endif