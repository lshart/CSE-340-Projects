#include "Symbol.h"
#include "Rule.h"

class SymbolTable
{
private:
	enum MyEnum	{ EPSILON = 0, EnOF = 1	};
	std::vector<Symbol*> *symbolList;
	std::vector<Rule*> *ruleList;
	std::vector<bool> *canLengthOne;
	int countOfSpecialSymbols;
	bool addFirstToFollowTo(Symbol *thisSymbol, Symbol *fromThisSymbol);
	bool addFirstSetsTo(Symbol *thisSymbol, Symbol *fromThisSymbol);
	bool addFollowSetsTo(Symbol *thisSymbol, Symbol *fromThisSymbol);
	bool computeCanLengthOne();
	void applyFirstSetRuleOne();
	void applyFirstSetRuleTwo();
	bool applyFirstSetRuleThreeFourFive();
	void applyFollowSetRuleOne();
	bool applyFollowSetRuleTwoThree();
	void applyFollowSetRuleFourFive();
	std::string printFirstSetOf(int index);
	std::string printFollowSetOf(int index);
	std::string printCanSingletOf(int index);

public:
	SymbolTable();
	int getCountOfSpecialSymbols();
	int findSymbolIDBy(std::string thisString);
	void addRule(Rule* newRule);
	void addTerminal(std::string newTerminal);
	void addNonTerminal(std::string newNonTerminal);
	void computeFirstSets();
	void computeFollowSets();
	bool isTerminalAtIndex(int thisIndex);
	void printFirstSets();
	void printFollowSets();
	void printLengthOne();
	// Some Test Functions
	void printRuleTable();
};