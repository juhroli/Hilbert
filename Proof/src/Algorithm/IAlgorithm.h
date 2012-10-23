#pragma once

#include "../HilbertIncludes.h"
#include "../Formula/IFormula.h"
#include "../Formula/Containers/FormulaSet.h"
#include "../Formula/Containers/AxiomContainer.h"

class IAlgorithm
{
public:
	virtual ~IAlgorithm() {}
	virtual void Start() = 0;
	virtual void SetAxioms(AxiomContainer * container) = 0;
	virtual void SetTask(FormulaSet * Sigma, IFormula * F) = 0;
};