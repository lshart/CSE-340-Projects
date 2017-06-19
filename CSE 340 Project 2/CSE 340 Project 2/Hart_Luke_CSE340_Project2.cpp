#include <iostream>
#include <string>
#include "SymbolTable.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		cout << "Error: missing argument\n";
		return 1;
	}

	int argument = atoi(argv[1]);

	string inStr;
	SymbolTable *grammarTable = new SymbolTable();

	cin >> inStr;

	while (inStr.compare("#") != 0)
	{
		grammarTable->addNonTerminal(inStr);
		cin >> inStr;
	}
	
	cin >> inStr;

	while (inStr.compare("##") != 0)
	{
		int LHSIndex = grammarTable->findSymbolIDBy(inStr);
		if (LHSIndex < grammarTable->getCountOfSpecialSymbols() || grammarTable->isTerminalAtIndex(LHSIndex))
		{
			cout << "Error: Attempted to create grammar rule for invalid symbol";
			return 0;
		}
		Rule* newRule = new Rule(LHSIndex);

		cin >> inStr;

		if (inStr.compare("->") != 0)
		{
			cout << "Error: Rule did not follow specified input format";
			return 0;
		}
		else
		{
			cin >> inStr;
		}

		if (inStr.compare("#") == 0)
		{
			newRule->addRHS(0);
		}
		else
		{
			while (inStr.compare("#") != 0)
			{
				grammarTable->addTerminal(inStr);
				int RHSIndex = grammarTable->findSymbolIDBy(inStr);
				newRule->addRHS(RHSIndex);
				cin >> inStr;
			}
		}
		grammarTable->addRule(newRule);

		cin >> inStr;
	}

	grammarTable->computeFollowSets();

	switch (argument)
	{
	case 1:
		grammarTable->printLengthOne();
		break;
	case 2:
		grammarTable->printFirstSets();
		break;
	case 3:
		grammarTable->printFollowSets();
		break;
	default:
		cout << "Invalid Argument";
	}
	return 0;
}