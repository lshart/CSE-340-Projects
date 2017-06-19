#ifndef TYPETABLE_H
#define TYPETABLE_H

#include"Entry.h"
#include<vector>

#define BUILT_IN_TYPES 14
enum BuiltIn { MUH_INT = 10, MUH_REAL, MUH_STRING, MUH_BOOLEAN, MUH_LONG };

enum TokenTypes
{
	VAR = 1, WHILE, INT, REAL, STRING, BOOLEAN,
	TYPE, LONG, DO, CASE, SWITCH,
	PLUS, MINUS, DIV, MULT, EQUAL,
	COLON, COMMA, SEMICOLON,
	LBRAC, RBRAC, LPAREN, RPAREN, LBRACE, RBRACE,
	NOTEQUAL, GREATER, LESS, LTEQ, GTEQ, DOT,
	ID, NUM, REALNUM,
	ERROR
};

class TypeTable
{
private:
	int TypeCount;
	std::vector<Entry*> *list;
		// decomposition functions
	void decompDeclare(struct declNode* name);
	void decompTypeDeclareSection(struct type_decl_sectionNode* name);
	void decompVarDeclareSection(struct var_decl_sectionNode* name);
	void decompTypeDeclareList(struct type_decl_listNode* name);
	void decompVarDeclareList(struct var_decl_listNode* name);
	void decompTypeDeclare(struct type_declNode* name);
	void decompVarDeclare(struct var_declNode* name);
	void decompBody(struct bodyNode* name);
	void decompStatementList(struct stmt_listNode* name);
	void decompStatement(struct stmtNode* name);
	void decompCondition(struct conditionNode* name);
	void decompWhile(struct while_stmtNode* name);
	void decompDoWhile(struct while_stmtNode* name);
	void decompAssign(struct assign_stmtNode* name);
	void decompCase(struct caseNode* name);
	void decompCaseList(struct case_listNode* name);
	void decompSwitch(struct switch_stmtNode* name);
		// Helper Functions
	int getExpressionTypeNumber(struct exprNode* name);
	int getPrimaryTypeNumber(struct primaryNode* name);
	int getIDListTypeNumber(struct id_listNode* name, bool var);
	int getTypeNameTypeNumber(struct type_nameNode* name, bool imp);
	int getIDTypeNumber(char* name, bool var, bool imp);
	int unifyTypeNumber(int t1, int t2);
	void updateTypeNumberOf(int thisOne, int toThisOne);
	int listAlreadyHas(char* thisString);
	void errorControl(char* name, bool var);
public:
	TypeTable();
	void processParseTree(struct programNode* program);
	void printTypeTable();
};

#endif