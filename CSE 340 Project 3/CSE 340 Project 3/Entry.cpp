#include"Entry.h"
#include<string>

using namespace std;

Entry::Entry(string nam, int tNum, int i, bool var, bool imp)
{
	eName = nam;
	typeNum = tNum;
	index = i;
	implicit = imp;
	variable = var;
}

string Entry::toString()
{
	return eName;
}

void Entry::setTypeNum(int newTypeNum)
{
	typeNum = newTypeNum;
}

int Entry::getIndex()
{
	return index;
}

int Entry::getTypeNum()
{
	return typeNum;
}

bool Entry::isImplicit()
{
	return implicit;
}

bool Entry::isVariable()
{
	return variable;
}