#include<vector>

class Rule
{
private:
	int LHS;
	std::vector<int>* RHS;

public:
	Rule(int newLHS);
	void addRHS(int newRHS);
	int getLHS();
	std::vector<int>* getRHS();
};