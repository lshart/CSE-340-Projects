#include "Symbol.h"
using namespace std;

Symbol::Symbol(bool ter, string rep)
{
	static int nextId = 0;
	terminal = ter;
	representation = rep;
	id = nextId;
	nextId++;
	firstSet = new vector<int>();
	followSet = new vector<int>();
}

string Symbol::toString()
{
	return representation;
}

bool Symbol::isTerminal()
{
	return terminal;
}

int Symbol::getID()
{
	return id;
}

vector<int>* Symbol::getFirstSet()
{
	return firstSet;
}

vector<int>* Symbol::getFollowSet()
{
	return followSet;
}

bool Symbol::isInFirstSet(int thisId)
{
	bool isInFirst = false;

	for (unsigned int i = 0; i < firstSet->size(); i++)
	{
		if (thisId == firstSet->at(i))
			isInFirst = true;
	}

	return isInFirst;
}

bool Symbol::isInFollowSet(int thisId)
{
	bool isInFollow = false;

	for (unsigned int i = 0; i < followSet->size(); i++)
	{
		if (thisId == followSet->at(i))
			isInFollow = true;
	}

	return isInFollow;
}

bool Symbol::addFirst(int thisId)
{
	if (!isInFirstSet(thisId))
	{
		firstSet->push_back(thisId);
		return true;
	}
	else
		return false;
}

bool Symbol::addFollow(int thisId)
{
	if (!isInFollowSet(thisId))
	{
		followSet->push_back(thisId);
		return true;
	}
	else
		return false;
}

bool Symbol::firstHasEpsilon()
{
	return isInFirstSet(0);
}