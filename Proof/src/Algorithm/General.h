#pragma once

class IFormula;
class ImplicationFormula;
class FormulaSet;

namespace General
{
	//empéző függvény, viszatér a levágott jobb oldallal
	IFormula * mp(IFormula& f, ImplicationFormula& impF);
	//Input imp formulának a bal oldalát belerakja a halmazba, visszatér a jobb oldalával
	IFormula * deduct(ImplicationFormula& f, FormulaSet& sigma);
}