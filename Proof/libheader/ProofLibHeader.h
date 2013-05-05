#ifndef HilbertIncludes_h
#define HilbertIncludes_h

#include <memory>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <ctime>

#pragma warning (disable : 4267)

#define DELETEFORMULA(x) { if(x != nullptr && (!x->IsAtomic() || x->IsWrapped())) { delete x; } x = nullptr; }
#define DELETE(x) if(x != nullptr) { delete x; x = nullptr; }
#define CPAIR(x, y) (((x + y) * (x + y + 1)) / 2 + y)
#define FALSE "~"
#define IMPLIES "->"

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
using std::list;
using std::string;
using std::pair;

class IFormula;
class AtomicFormula;
class TempFormula;
class ImplicationFormula;
class Axiom;
class FormulaWrapper;
class FileReader;

typedef shared_ptr<IFormula> spIFormula;
typedef shared_ptr<AtomicFormula> spAtomicFormula;
typedef shared_ptr<TempFormula> spTempFormula;
typedef shared_ptr<ImplicationFormula> spImplicationFormula;
typedef shared_ptr<Axiom> spAxiom;

enum FSetType
{
	FSET_VEC,
	FSET_LIST,
	FSET_HASHED
};

class IFormula
{
public:
	virtual ~IFormula() {}
	virtual bool IsAtomic() = 0;
	virtual bool IsTemp() = 0;
	virtual bool Eval() = 0;
	virtual bool Equals(IFormula * formula) = 0;
	virtual string ToString() = 0;
	virtual IFormula * Clone() = 0;
	virtual IFormula * Replace(IFormula * x, IFormula * t) = 0;
	virtual unsigned Length() = 0;
	virtual long HashCode() = 0;
	virtual bool IsWrapped() = 0;
};

namespace AFormulaTable
{
	spAtomicFormula GetAtomicFormula(char * symbol);
	spTempFormula GetTempFormula(char * symbol);
	spAtomicFormula GetAtomicFormula(long hash);
	spAtomicFormula AddAtomicFormula(AtomicFormula * formula);
	spAtomicFormula AddAtomicFormula(char * symbol);
	spTempFormula AddTempFormula(char * symbol);

	void SetDefaults();
	void DestroyTable();
	string ListAtomicFormulas();

	unordered_map<long, spAtomicFormula>::iterator TableBegin();
	unordered_map<long, spAtomicFormula>::iterator TableEnd();
}

class AxiomContainer
{
public:
	AxiomContainer();
	virtual ~AxiomContainer();

	virtual Axiom * GetAxiom(unsigned index);
	void Add(IFormula * axiom);
	unsigned Size();

	virtual string ToString();

protected:
	vector<Axiom*> m_axioms;
};

class HilbertAxioms : public AxiomContainer
{
public:
	HilbertAxioms();
	~HilbertAxioms();
	Axiom * GetAxiom(unsigned index);
};

class IFormulaSet
{
public:
	virtual ~IFormulaSet() {}

	virtual void Add(IFormula * formula) = 0;
	virtual void Add(spIFormula formula) = 0;
	virtual void Add(IFormulaSet& fset) = 0;

	virtual unsigned Size() = 0;
	virtual bool Contains(long hash) = 0;
	virtual spIFormula Get(long hash) = 0;
	virtual void Clear() = 0;
	virtual string ToString() = 0;

protected:
	unordered_map<long, spIFormula> m_formulaMap;
};

namespace FormulaParser
{
	IFormula * ParseFormula(string str);
	IFormula * ParseTemp(string str);

	IFormulaSet * ParseFormulaSet(string str, FSetType type);
	AxiomContainer * ParseAxioms(string str);
}

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
	int m_startSize;
	int m_endSize;
	unsigned m_MPCount;
	unsigned m_axiomCount;

protected:
	void Stat_StartTimer();
	void Stat_EndTimer();
	void Stat_StartSize(int n);
	void Stat_EndSize(int n);

	void Stat_incMPCount();
	void Stat_incAxiomCount();
};

class AlgorithmBase : public Stat
{
public:
	AlgorithmBase();
	virtual ~AlgorithmBase();
	
	virtual void SetTask(IFormulaSet * Sigma, IFormula * F) = 0;
	virtual FSetType GetFSetType() = 0;
	virtual void SetAxioms(AxiomContainer * container);
	virtual bool IsFinished();
	virtual string GetResult();

	void Start();
	bool ReadFromFile(string file);
	void SetMaxLength(unsigned length);
	void SetSigmaLimit(unsigned limit);
	void SetLatex(bool use);
	
protected:
    AxiomContainer * m_axioms;
    IFormulaSet * m_sigma;
    IFormula * m_target;
	bool m_finished;
	FormulaWrapper * m_last;
	unsigned m_maxLength;
	unsigned m_sigmaLimit;
	string m_resString;
	string m_taskString;
	FileReader * m_reader;
	bool m_useLatex;

	bool MPBothWays(IFormula * a, IFormula * b, IFormulaSet*& fset);

	virtual void Run() = 0;
	virtual string ResultString();
	virtual void DoDeduction();
	virtual void AddAxiomsToSigma();
};

namespace General
{
	enum AlgorithmType
	{
		ALG_0x00,
		ALG_0x01,
		ALG_0x02,
		ALG_0x03
	};

	AlgorithmBase * CreateAlgorithm(AlgorithmType type);
	AlgorithmBase * CreateAlgorithm(AlgorithmType type, unsigned parm);
	string AlgorithmsDescription();
}

#endif
