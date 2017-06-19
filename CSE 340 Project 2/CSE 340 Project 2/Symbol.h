#include <string>
#include <vector>

class Symbol
{
private:
	bool terminal;
	std::string representation;
	int id;
	std::vector<int> *firstSet;
	std::vector<int> *followSet;

public:
	Symbol(bool ter, std::string rep);
	std::string toString();
	int getID();
	std::vector<int>* getFirstSet();
	std::vector<int>* getFollowSet();
	bool isInFirstSet(int thisId);
	bool isInFollowSet(int thisId);
	bool addFirst(int thisId);
	bool addFollow(int thisId);
	bool firstHasEpsilon();
	bool isTerminal();
};