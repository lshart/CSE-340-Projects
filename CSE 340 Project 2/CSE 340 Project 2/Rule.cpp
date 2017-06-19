#include "Rule.h"
using namespace std;

Rule::Rule(int newLHS)
{
	LHS = newLHS;
	RHS = new vector<int>();
}

void Rule::addRHS(int newRHS)
{
	RHS->push_back(newRHS);
}

vector<int>* Rule::getRHS()
{
	return RHS;
}

int Rule::getLHS()
{
	return LHS;
}