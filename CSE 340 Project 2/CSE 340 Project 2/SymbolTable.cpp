#include "SymbolTable.h"
#include <iostream>
#include <algorithm>
using namespace std;

SymbolTable::SymbolTable()
{
	symbolList = new vector<Symbol*>();
	ruleList = new vector<Rule*>();
	canLengthOne = new vector<bool>();

	countOfSpecialSymbols = 0;

	symbolList->push_back(new Symbol(false, "#"));
	countOfSpecialSymbols++;
	canLengthOne->push_back(false);
	symbolList->push_back(new Symbol(false, "$"));
	countOfSpecialSymbols++;
	canLengthOne->push_back(false);
}

void SymbolTable::addRule(Rule* newRule)
{
	ruleList->push_back(newRule);
}

int SymbolTable::getCountOfSpecialSymbols()
{
	return countOfSpecialSymbols;
}

int SymbolTable::findSymbolIDBy(string thisString)
{
	int index = -1;

	for (unsigned int i = 0; i < symbolList->size(); i++)
	{
		if (thisString.compare(symbolList->at(i)->toString()) == 0)
			index = i;
	}

	return index;
}

void SymbolTable::addTerminal(string newTerminal)
{
	if (findSymbolIDBy(newTerminal) < 0)
	{
		canLengthOne->push_back(false);
		symbolList->push_back(new Symbol(true, newTerminal));
	}
}

void SymbolTable::addNonTerminal(string newNonTerminal)
{
	if (findSymbolIDBy(newNonTerminal) < 0)
	{
		canLengthOne->push_back(false);
		symbolList->push_back(new Symbol(false, newNonTerminal));
	}
}

bool SymbolTable::isTerminalAtIndex(int thisIndex)
{
	return symbolList->at(thisIndex)->isTerminal();
}

void SymbolTable::computeFirstSets()
{
	applyFirstSetRuleOne();
	applyFirstSetRuleTwo();
	while (applyFirstSetRuleThreeFourFive()) { }
}

void SymbolTable::computeFollowSets()
{
	computeFirstSets();
	applyFollowSetRuleOne();
	applyFollowSetRuleFourFive();
	while (applyFollowSetRuleTwoThree()) { }
}

void SymbolTable::printFirstSets()
{
	for (unsigned int i = countOfSpecialSymbols; i < symbolList->size(); i++)
		if (!symbolList->at(i)->isTerminal())
			cout << printFirstSetOf(i) << endl;
}

void SymbolTable::printFollowSets()
{
	for (unsigned int i = countOfSpecialSymbols; i < symbolList->size(); i++)
		if (!symbolList->at(i)->isTerminal())
			cout << printFollowSetOf(i) << endl;
}

void SymbolTable::printLengthOne()
{
	while (computeCanLengthOne()) {	}

	for (unsigned int i = countOfSpecialSymbols; i < symbolList->size(); i++)
		if (!symbolList->at(i)->isTerminal())
			cout << printCanSingletOf(i) << endl;
}

// Helper functions
bool SymbolTable::addFirstSetsTo(Symbol *thisSymbol, Symbol *fromThisSymbol)
{
	bool addedSets = false;
	vector<int> *setToAdd = fromThisSymbol->getFirstSet();
	if (!thisSymbol->isTerminal())
	{
		for (unsigned int i = 0; i < setToAdd->size(); i++)
		{
			if (setToAdd->at(i) >= countOfSpecialSymbols)
			{
				if (thisSymbol->addFirst(setToAdd->at(i)))
					addedSets = true;
			}
		}
	}
	return addedSets;
}

bool SymbolTable::addFollowSetsTo(Symbol *thisSymbol, Symbol *fromThisSymbol)
{
	bool addedSets = false;
	vector<int> *setToAdd = fromThisSymbol->getFollowSet();

	if (!thisSymbol->isTerminal())
	{
		for (unsigned int i = 0; i < setToAdd->size(); i++)
		{
			if (setToAdd->at(i) >= countOfSpecialSymbols - 1)
			{
				if (thisSymbol->addFollow(setToAdd->at(i)))
				addedSets = true;
			}
		}
	}
	return addedSets;
}

bool SymbolTable::addFirstToFollowTo(Symbol *thisSymbol, Symbol *fromThisSymbol)
{
	bool addedSets = false;
	vector<int> *setToAdd = fromThisSymbol->getFirstSet();
	if (!thisSymbol->isTerminal())
	{
		for (unsigned int i = 0; i < setToAdd->size(); i++)
		{
			if (setToAdd->at(i) >= countOfSpecialSymbols)
			{
				if (thisSymbol->addFollow(setToAdd->at(i)))
					addedSets = true;
			}
		}
	}
	return addedSets;
}

bool SymbolTable::computeCanLengthOne()
{
	bool changed = false;
	for (unsigned int i = 0; i < ruleList->size(); i++) // for each rule
	{
		int LHS = ruleList->at(i)->getLHS();
		vector<int>* RHS = ruleList->at(i)->getRHS();
		unsigned int epsilonCount = 0;
		int terminalCount = 0;
		int canSingletCount = 0;
		bool notSingletNotEpsilon = false;

		for (unsigned int j = 0; j < RHS->size(); j++) // for every element in RHS
		{
			if (symbolList->at(RHS->at(j))->firstHasEpsilon())
				epsilonCount++;
			if (symbolList->at(RHS->at(j))->isTerminal())
				terminalCount++;
			if (canLengthOne->at(RHS->at(j)))
				canSingletCount++;
			else if (!symbolList->at(RHS->at(j))->firstHasEpsilon())
				notSingletNotEpsilon = true;
		}

		if (terminalCount <= 1)
		{
			if (epsilonCount >= RHS->size() - 1)
			{
				if (terminalCount == 1 || (canSingletCount >= 1 && !notSingletNotEpsilon))
				{
					if (!canLengthOne->at(LHS))
					{
						changed = true;
						canLengthOne->at(LHS) = true;
					}
				}
			}
		}
	}

	return changed;
}

string SymbolTable::printFirstSetOf(int index)
{
	Symbol *thisSymbol = symbolList->at(index);
	vector<int> *set = thisSymbol->getFirstSet();
	int startOfSort = 0;

	vector<string> *setToString = new vector<string>();

	for (unsigned int i = 0; i < set->size(); i++)
	{
		if (set->at(i) == EPSILON)
		{
			setToString->push_back(symbolList->at(EPSILON)->toString());
			startOfSort++;
		}
	}

	for (unsigned int i = 0; i < set->size(); i++)
	{
		if (set->at(i) != EPSILON)
			setToString->push_back(symbolList->at(set->at(i))->toString());
	}

	sort(setToString->begin() + startOfSort, setToString->end());

	string result = "";

	result.append("FIRST(");
	result.append(thisSymbol->toString());
	result.append(") = { ");

	for (unsigned int i = 0; i < setToString->size(); i++)
	{
		result.append(setToString->at(i));
		if (i < setToString->size() - 1)
			result.append(", ");
	}
	result.append(" }");

	return result;
}

string SymbolTable::printFollowSetOf(int index)
{
	Symbol *thisSymbol = symbolList->at(index);
	vector<int> *set = thisSymbol->getFollowSet();
	int startOfSort = 0;

	vector<string> *setToString = new vector<string>();

	for (unsigned int i = 0; i < set->size(); i++)
	{
		if (set->at(i) == EnOF)
		{
			setToString->push_back(symbolList->at(EnOF)->toString());
			startOfSort++;
		}
	}

	for (unsigned int i = 0; i < set->size(); i++)
	{
		if (set->at(i) != EnOF)
			setToString->push_back(symbolList->at(set->at(i))->toString());
	}

	sort(setToString->begin() + startOfSort, setToString->end());

	string result = "";

	result.append("FOLLOW(");
	result.append(thisSymbol->toString());
	result.append(") = { ");

	for (unsigned int i = 0; i < setToString->size(); i++)
	{
		result.append(setToString->at(i));
		if (i < setToString->size() - 1)
			result.append(", ");
	}
	result.append(" }");

	return result;
}

string SymbolTable::printCanSingletOf(int index)
{
	string result = "";

	result.append(symbolList->at(index)->toString());
	result.append(": ");
	if (canLengthOne->at(index))
		result.append("YES");
	else
		result.append("NO");

	return result;
}

void SymbolTable::applyFirstSetRuleOne()
{
	for (unsigned int i = 0; i < symbolList->size(); i++)
	{
		if (symbolList->at(i)->isTerminal())
			symbolList->at(i)->addFirst(symbolList->at(i)->getID());
	}
}

void SymbolTable::applyFirstSetRuleTwo()
{
	symbolList->at(0)->addFirst(symbolList->at(EPSILON)->getID());
}

bool SymbolTable::applyFirstSetRuleThreeFourFive()
{
	bool changedSomething = false;

	for (unsigned int i = 0; i < ruleList->size(); i++) // for each rule
	{
		int LHS = ruleList->at(i)->getLHS();
		vector<int> *RHS = ruleList->at(i)->getRHS();
		bool stop = false;

		for (unsigned int j = 0; j < RHS->size() && !stop; j++)
		{
			if (symbolList->at(RHS->at(j))->isTerminal() || !symbolList->at(RHS->at(j))->firstHasEpsilon())
				stop = true;
			if (addFirstSetsTo(symbolList->at(LHS), symbolList->at(RHS->at(j))))
				changedSomething = true;
		}

		if (!stop)
			if (symbolList->at(LHS)->addFirst(EPSILON))
				changedSomething = true;
	}

	return changedSomething;
}

void SymbolTable::applyFollowSetRuleOne()
{
	symbolList->at(countOfSpecialSymbols)->addFollow(EnOF);
}

bool SymbolTable::applyFollowSetRuleTwoThree()
{
	bool changedSomething = false;
	for (unsigned int i = 0; i < ruleList->size(); i++) // for each rule
	{
		int LHS = ruleList->at(i)->getLHS();
		vector<int> *RHS = ruleList->at(i)->getRHS();
		bool stop = false;

		for (int j = RHS->size() - 1; j >= 0 && !stop; j--)
		{
			if (!symbolList->at(RHS->at(j))->isTerminal())
				if (addFollowSetsTo(symbolList->at(RHS->at(j)), symbolList->at(LHS)))
					changedSomething = true;

			if (!symbolList->at(RHS->at(j))->firstHasEpsilon() || symbolList->at(RHS->at(j))->isTerminal())
				stop = true;
		}
	}

	return changedSomething;
}

void SymbolTable::applyFollowSetRuleFourFive()
{
	for (unsigned int i = 0; i < ruleList->size(); i++) // for each rule
	{
		vector<int> *RHS = ruleList->at(i)->getRHS();

		for (unsigned int j = 0; j < RHS->size(); j++) // for each element in righthand side
		{
			bool stop = false;
			if (!symbolList->at(RHS->at(j))->isTerminal())
				for (unsigned int k = j + 1; k < RHS->size() && !stop ; k++) // for every element after the current
				{
					addFirstToFollowTo(symbolList->at(RHS->at(j)), symbolList->at(RHS->at(k)));
					if (!symbolList->at(RHS->at(k))->firstHasEpsilon())
						stop = true;
				}

		}
	}
}

// Test Functions
void SymbolTable::printRuleTable()
{
	for (unsigned int i = 0; i < ruleList->size(); i++)
	{
		cout << symbolList->at(ruleList->at(i)->getLHS())->toString() << " -> ";

		vector<int> *RHS = ruleList->at(i)->getRHS();
		if (RHS->empty())
		{
			cout << "# ";
		}
		else
		{
			for (unsigned int j = 0; j < RHS->size(); j++)
			{
				cout << symbolList->at(RHS->at(j))->toString() << " ";
			}
		}
		cout << endl;
	}
}