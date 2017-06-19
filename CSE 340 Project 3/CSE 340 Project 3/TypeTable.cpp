#include"TypeTable.h"
#include"syntax.h"
#include<iostream>

using namespace std;

TypeTable::TypeTable()
{
	list = new vector<Entry*>();
	TypeCount = BUILT_IN_TYPES + 1;

	list->push_back(new Entry("INT", MUH_INT, MUH_INT, false, false));
	list->push_back(new Entry("REAL", MUH_REAL, MUH_REAL, false, false));
	list->push_back(new Entry("STRING", MUH_STRING, MUH_STRING, false, false));
	list->push_back(new Entry("BOOLEAN", MUH_BOOLEAN, MUH_BOOLEAN, false, false));
	list->push_back(new Entry("LONG", MUH_LONG, MUH_LONG, false, false));
}

void TypeTable::processParseTree(struct programNode* program)
{
	decompDeclare(program->decl);
	decompBody(program->body);
}

bool hasFalse(vector<bool> *inHere)
{
	bool hasFalse = false;
	for (unsigned int i = 0; i < inHere->size(); i++)
		if (!inHere->at(i))
			hasFalse = true;
	return hasFalse;
}

void TypeTable::printTypeTable()
{
	vector<bool> *beenDisplayed = new vector<bool>();
	for (unsigned int i = 0; i < list->size(); i++)
		beenDisplayed->push_back(false);

	unsigned int index = 0;
	while (hasFalse(beenDisplayed))
	{
		beenDisplayed->at(index) = true;
		string printMe = "";
		int thisTypeNum = list->at(index)->getTypeNum();
		printMe += list->at(index)->toString() + " : ";
		bool provedUseful = false;
		for (unsigned int i = index + 1; i < list->size(); i++)
		{
			if (list->at(i)->getTypeNum() == thisTypeNum  && !beenDisplayed->at(i) && !list->at(i)->isVariable() && !list->at(i)->isImplicit())
			{
				provedUseful = true;
				beenDisplayed->at(i) = true;
				printMe += list->at(i)->toString() + " ";
			}
		}

		for (unsigned int i = index + 1; i < list->size(); i++)
		{
			if (list->at(i)->getTypeNum() == thisTypeNum  && !beenDisplayed->at(i) && !list->at(i)->isVariable() && list->at(i)->isImplicit())
			{
				provedUseful = true;
				beenDisplayed->at(i) = true;
				printMe += list->at(i)->toString() + " ";
			}
		}

		for (unsigned int i = index + 1; i < list->size(); i++)
		{
			if (list->at(i)->getTypeNum() == thisTypeNum  && !beenDisplayed->at(i) && list->at(i)->isVariable() && !list->at(i)->isImplicit())
			{
				provedUseful = true;
				beenDisplayed->at(i) = true;
				printMe += list->at(i)->toString() + " ";
			}
		}

		for (unsigned int i = index + 1; i < list->size(); i++)
		{
			if (list->at(i)->getTypeNum() == thisTypeNum  && !beenDisplayed->at(i) && list->at(i)->isVariable() && list->at(i)->isImplicit())
			{
				provedUseful = true;
				beenDisplayed->at(i) = true;
				printMe += list->at(i)->toString() + " ";
			}
		}
		printMe += "#";

		if (provedUseful)
			cout << printMe << endl;

		index++;
		if (index >= list->size())
			index = 0;
	}
}

	// Decomposition Functions
void TypeTable::decompDeclare(struct declNode* name)
{
	if (name->type_decl_section != NULL)
		decompTypeDeclareSection(name->type_decl_section);
	if (name->var_decl_section != NULL)
		decompVarDeclareSection(name->var_decl_section);
}

void TypeTable::decompTypeDeclareSection(struct type_decl_sectionNode* name)
{
	decompTypeDeclareList(name->type_decl_list);
}

void TypeTable::decompVarDeclareSection(struct var_decl_sectionNode* name)
{
	decompVarDeclareList(name->var_decl_list);
}

void TypeTable::decompTypeDeclareList(struct type_decl_listNode* name)
{
	decompTypeDeclare(name->type_decl);
	if (name->type_decl_list != NULL)
		decompTypeDeclareList(name->type_decl_list);
}

void TypeTable::decompVarDeclareList(struct var_decl_listNode* name)
{
	decompVarDeclare(name->var_decl);
	if (name->var_decl_list != NULL)
		decompVarDeclareList(name->var_decl_list);
}

void TypeTable::decompTypeDeclare(struct type_declNode* name)
{
	int listTypeNum = getIDListTypeNumber(name->id_list, false);
	int typeNameTypeNum = getTypeNameTypeNumber(name->type_name, true);

	unifyTypeNumber(typeNameTypeNum, listTypeNum);
}

void TypeTable::decompVarDeclare(struct var_declNode* name)
{
	errorControl(name->id_list->id, true);

	int listTypeNum = getIDListTypeNumber(name->id_list, true);

	if (name->type_name->type == ID)
	{
		int index = listAlreadyHas(name->type_name->id);
		if (index > 0)
			if (list->at(index)->isVariable())
				errorControl(name->type_name->id, false);
	}
	int typeNameTypeNum = getTypeNameTypeNumber(name->type_name, true);

	unifyTypeNumber(typeNameTypeNum, listTypeNum);
}


void TypeTable::decompBody(struct bodyNode* name)
{
	decompStatementList(name->stmt_list);
}

void TypeTable::decompStatementList(struct stmt_listNode* name)
{
	decompStatement(name->stmt);
	if (name->stmt_list != NULL)
		decompStatementList(name->stmt_list);
}

void TypeTable::decompStatement(struct stmtNode* name)
{
	switch (name->stmtType)
	{
	case WHILE:
		decompWhile(name->while_stmt);
		break;
	case DO:
		decompDoWhile(name->while_stmt);
		break;
	case SWITCH:
		decompSwitch(name->switch_stmt);
		break;
	case ASSIGN:
		decompAssign(name->assign_stmt);
		break;
	default:
		cout << "Something broke in decompStatement.\n";
		exit(0);
		break;
	}
}

void TypeTable::decompCondition(struct conditionNode* name)
{
	int thisShouldBeABoolean;
	int leftSide;
	int rightSide;

	switch (name->relop)
	{
	case ID:
		thisShouldBeABoolean = getPrimaryTypeNumber(name->left_operand);
		unifyTypeNumber(MUH_BOOLEAN, thisShouldBeABoolean);
		break;
	case NOTEQUAL:
	case GREATER:
	case LESS:
	case LTEQ:
	case GTEQ:
		leftSide = getPrimaryTypeNumber(name->left_operand);
		rightSide = getPrimaryTypeNumber(name->right_operand);
		unifyTypeNumber(leftSide, rightSide);
		break;
	default:
		cout << "Something broke in decompCondition.\n";
		exit(0);
		break;
	}
}

void TypeTable::decompWhile(struct while_stmtNode* name)
{
	decompCondition(name->condition);
	decompBody(name->body);
}

void TypeTable::decompDoWhile(struct while_stmtNode* name)
{
	decompBody(name->body);
	decompCondition(name->condition);
}

void TypeTable::decompAssign(struct assign_stmtNode* name)
{
	int id = getIDTypeNumber(name->id, true, true);
	int exp = getExpressionTypeNumber(name->expr);

	unifyTypeNumber(id, exp);
}

void TypeTable::decompCase(struct caseNode* name)
{
	decompBody(name->body);
}

void TypeTable::decompCaseList(struct case_listNode* name)
{
	decompCase(name->cas);
	if (name->case_list != NULL)
		decompCaseList(name->case_list);
}

void TypeTable::decompSwitch(struct switch_stmtNode* name)
{
	int id = getIDTypeNumber(name->id, true, true);
	unifyTypeNumber(MUH_INT, id);

	decompCaseList(name->case_list);
}

	// Helper Functions
int TypeTable::getExpressionTypeNumber(struct exprNode* name)
{
	if (name->tag == PRIMARY)
	{
		return getPrimaryTypeNumber(name->primary);
	}
	else if (name->tag == EXPR)
	{
		int left = getExpressionTypeNumber(name->leftOperand);
		int right = getExpressionTypeNumber(name->rightOperand);

		return unifyTypeNumber(left, right);
	}
	else
	{
		cout << "Something broke in getExpresionTypeNumber.\n";
		exit(0);
	}
	return NULL;
}

int TypeTable::getPrimaryTypeNumber(struct primaryNode* name)
{
	int retType = -1;
	switch (name->tag)
	{
	case NUM:
		retType = MUH_INT;
		break;
	case REALNUM:
		retType = MUH_REAL;
		break;
	case ID:
		retType = getIDTypeNumber(name->id, true, true);
		break;
	default:
		cout << "Something broke in getPrimaryTypeNumber.\n";
		exit(0);
		break;
	}
	return retType;
}

int TypeTable::getIDListTypeNumber(struct id_listNode* name, bool var)
{
	errorControl(name->id, var);
	int idType = getIDTypeNumber(name->id, var, false);
	int listType = idType;
	if (name->id_list != NULL)
		listType = getIDListTypeNumber(name->id_list, var);

	return unifyTypeNumber(idType, listType);
}

int typeToMuhType(int type)
{
	switch (type)
	{
	case INT:
		return MUH_INT;
	case REAL:
		return MUH_REAL;
	case STRING:
		return MUH_STRING;
	case BOOLEAN:
		return MUH_BOOLEAN;
	case LONG:
		return MUH_LONG;
	default:
		cout << "Something broke in MuhTypes!!!\n";
		exit(0);
	}
}

int TypeTable::getTypeNameTypeNumber(struct type_nameNode* name, bool imp)
{
	if (name->type == ID)
		return getIDTypeNumber(name->id, false, imp);
	else
		return typeToMuhType(name->type);
}

int TypeTable::getIDTypeNumber(char* name, bool var, bool imp)
{
	int muhReturn = -1;
	for (int i = 0; i < (int) list->size(); i++)
	{
		if (list->at(i)->toString().compare(name) == 0)
			return list->at(i)->getTypeNum();
	}
	if (muhReturn < 0)
	{
		list->push_back(new Entry(name, TypeCount, TypeCount, var, imp));
		TypeCount++;
		return TypeCount - 1;
	}
	else
	{
		cout << "Something broke in getIDTypeNumber.\n";
		exit(0);
	}
	return NULL;
}

int TypeTable::unifyTypeNumber(int t1, int t2)
{
	if (t1 == t2)
	{
		return t1;
	}
	else if (t1 <= BUILT_IN_TYPES || t2 <= BUILT_IN_TYPES)
	{
		if (t1 <= BUILT_IN_TYPES && t2 <= BUILT_IN_TYPES)
		{
			cout << "ERROR CODE 3\n";
			exit(0);
		}
		else if (t1 <= BUILT_IN_TYPES)
		{
			updateTypeNumberOf(t2, t1);
			return t1;
		}
		else
		{
			updateTypeNumberOf(t1, t2);
			return t2;
		}
	}
	else
	{
		updateTypeNumberOf(t2, t1);
		return t1;
	}
	return NULL;
}

void TypeTable::updateTypeNumberOf(int thisOne, int toThisOne)
{
	for (unsigned int i = 0; i < list->size(); i++)
	{
		if (list->at(i)->getTypeNum() == thisOne)
			list->at(i)->setTypeNum(toThisOne);
	}
}

int TypeTable::listAlreadyHas(char* thisString)
{
	int index = -1;
	for (unsigned int i = 0; i < list->size(); i++)
	{
		if (list->at(i)->toString().compare(thisString) == 0)
			index = i;
	}

	return index;
}

void TypeTable::errorControl(char* name, bool var)
{
	int index = listAlreadyHas(name);
	if (index >= 0)
	{
		if (var && list->at(index)->isVariable())
		{
			cout << "ERROR CODE 2\n";
			exit(0);
		}
		else if (var && !list->at(index)->isVariable())
		{
			cout << "ERROR CODE 1\n";
			exit(0);
		}
		else if (!var && !list->at(index)->isVariable())
		{
			cout << "ERROR CODE 0\n";
			exit(0);
		}
		else if (!var && list->at(index)->isVariable())
		{
			cout << "ERROR CODE 4\n";
			exit(0);
		}
//		else
//		{
	//		cout << "Something broke in errorControl (go figure).\n";
		//	exit(0);
		//}
	}
}