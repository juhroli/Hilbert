#ifndef Stat_H
#define Stat_H

#include "../HilbertIncludes.h"

#include <ctime>
#include <string>

using std::string;

class Stat
{
public:
	Stat();
	~Stat() {}

	void Stat_StartTimer();
	void Stat_EndTimer();
	double Stat_GetElapsedTime();
	
	void Stat_StartSize(int n);
	void Stat_EndSize(int n);
	int Stat_GetSizeDelta();

	void Stat_incMPCount();
	void Stat_incUnificationCount();
	void Stat_incAxiomCount();

	string Stat_ToString();

private:
	clock_t m_startTime;
	clock_t m_endTime;
	double m_elapsedTime;
	int m_startSize; //number of formulas in set in the beginning
	int m_endSize; //number of formulas in set in the end
	unsigned m_MPCount; //number of modus ponenses
	unsigned m_unificationCount; //number of unifications executed
	unsigned m_axiomCount;
};


#endif