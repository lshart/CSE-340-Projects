#ifndef ENTRY_H
#define ENTRY_H

#include<string>

class Entry
{
private:
	std::string eName;
	int typeNum;
	int index; // going to treat this as the "original" type number
	bool implicit;
	bool variable;
public:
	Entry(std::string nam, int tNum, int i, bool var, bool imp);
	std::string toString();
	void setTypeNum(int newTypeNum);
	int getIndex();
	int getTypeNum();
	bool isImplicit();
	bool isVariable();
};

#endif