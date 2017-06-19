#include"TypeTable.h"

extern "C"
{
	struct programNode* program();
	void print_parse_tree(struct programNode* program);
}

int main()
{
	TypeTable *myTable = new TypeTable();
	struct programNode* parseTree;
	parseTree = program();

	myTable->processParseTree(parseTree);
	myTable->printTypeTable();
	return 0;
}



//  > "$(ProjectDir)tests/output.txt"