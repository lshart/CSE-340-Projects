#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include"compiler.h"
#include"GhettoCompile.h"

#ifdef _MSC_VER 
//#define atoi _atoi
//#define strcpy strcpy_s
#endif

#define ALLOC(t) (t*) calloc(1, sizeof(t))

#define MAX_VARS 1000		// I'm hoping that I don't need to make mroe that 1000 variables
struct ValueNode* varList[MAX_VARS];
int numVars = 0;

// I hope you don't mind that I've borrowed some things from you, such as this syntax error function
void syntax_error(const char* msg)
{
	printf("Syntax error while parsing %s line %d\n", msg, 0);
	exit(1);
}

struct IfStatement* case_stmt(struct StatementNode* target, struct ValueNode* test)
{
	struct IfStatement* casNo;

	ttype = getToken();
	if (ttype == CASE)
	{
		casNo = ALLOC(struct IfStatement);
		ttype = getToken();
		if (ttype == NUM)
		{
			ungetToken();
			casNo->condition_operand1 = test;
			casNo->condition_op = NOTEQUAL;
			casNo->condition_operand2 = number();
			ttype = getToken();
			if (ttype == COLON)
			{
				casNo->true_branch = NULL;
				casNo->false_branch = body(target);
				return casNo;
			}
			else
				syntax_error("case. COLON expected");
		}
		else
			syntax_error("case. NUM expected");
	}
	else
		syntax_error("case. CASE expected");
	return NULL;
}

struct StatementNode* stmt()
{
	struct StatementNode* statement;
	struct StatementNode* tempNoOp;

	ttype = getToken();
	if (ttype == ID || ttype == PRINT || ttype == WHILE || ttype == IF || ttype == SWITCH)
	{
		statement = ALLOC(struct StatementNode);
		if (ttype == ID)
		{
			ungetToken();
			statement->type = ASSIGN_STMT;
			statement->assign_stmt = assn_stmt();
			statement->next = NULL;
			return statement;
		}
		else if (ttype == PRINT)
		{
			ungetToken();
			statement->type = PRINT_STMT;
			statement->print_stmt = print_stmt();
			statement->next = NULL;
			return statement;
		}
		else if (ttype == WHILE)
		{
			ungetToken();
			tempNoOp = no_op_stmt();
			statement->type = IF_STMT;
			statement->next = tempNoOp;
			statement->if_stmt = while_stmt(tempNoOp, statement);
			return statement;
		}
		else if (ttype == IF)
		{
			ungetToken();
			tempNoOp = no_op_stmt();
			statement->type = IF_STMT;
			statement->next = tempNoOp;
			statement->if_stmt = if_stmt(tempNoOp);
			return statement;
		}
		else if (ttype == SWITCH)
		{
			ungetToken();
			free(statement);
			tempNoOp = no_op_stmt();
			statement = switch_stmt(tempNoOp);
			statement->next = tempNoOp;
			return statement;
		}
		else
		{
			syntax_error("stmt. Inside: ID, PRINT, WHILE, IF, or SWITCH expected");
		}
	}
	else
	{
		syntax_error("stmt. ID, PRINT, WHILE, IF, or SWITCH expected");
	}
	return NULL;
}

struct ValueNode* number()
{
	struct ValueNode* num;

	ttype = getToken();
	if (ttype == NUM)
	{
		num = ALLOC(struct ValueNode);
		num->name = "CONST";
		num->value = atoi(token);
		return num;
	}
	else
	{
		syntax_error("number. NUM expected");
	}
	return NULL;
}

struct StatementNode* body(struct StatementNode* next)
{
	struct StatementNode* bod;

	ttype = getToken();
	if (ttype == LBRACE)
	{
		bod = stmt_list(next);

		ttype = getToken();
		if (ttype == RBRACE)
			return bod;
		else
			syntax_error("body. RBRACE expected");
	}
	else
		syntax_error("body. LBRACE expected");
	return NULL;
}

struct AssignmentStatement* assn_stmt()
{
	struct AssignmentStatement* assn;

	ttype = getToken();
	if (ttype == ID)
	{
		assn = ALLOC(struct AssignmentStatement);
		assn->left_hand_side = var(token);

		ttype = getToken();
		if (ttype == EQUAL)
		{
			ttype = getToken();
			if (ttype == ID || ttype == NUM)
			{
				ungetToken();
				assn->operand1 = primary();
				assn->operand2 = NULL;
				assn->op = 0;

				ttype = getToken();
				if (ttype == PLUS || ttype == MINUS || ttype == DIV || ttype == MULT)
				{
					assn->op = ttype;
					assn->operand2 = primary();

					ttype = getToken();
					if (ttype == SEMICOLON)
					{
						return assn;
					}
					else
						syntax_error("assn_stmt. SEMICOLON expected");
				}
				else if (ttype == SEMICOLON)
					return assn;
				else
					syntax_error("assn_stmt. SEMICOLON expected");

			}
			else
				syntax_error("assn_stmt. ID, or NUM expected");
		}
		else
			syntax_error("assn_stmt. EQUAL expected");
	}
	else
		syntax_error("assn_stmt. ID expected");
	return NULL;
}

struct PrintStatement* print_stmt()
{
	struct PrintStatement* prin;
	ttype = getToken();

	if (ttype == PRINT)
	{
		ttype = getToken();
		if (ttype == ID)
		{
			prin = ALLOC(struct PrintStatement);
			prin->id = var(token);

			ttype = getToken();
			if (ttype == SEMICOLON)
				return prin;
			else
				syntax_error("print_stmt. SEMICOLON expected");
		}
		else
			syntax_error("print_stmt. ID expected");
	}
	else
		syntax_error("print_stmt. print expected");
	return NULL;
}

struct IfStatement* if_stmt(struct StatementNode* noop)
{
	struct IfStatement* if_st;

	ttype = getToken();
	if (ttype == IF)
	{
		if_st = ALLOC(struct IfStatement);
		ttype = getToken();
		if (ttype == NUM || ttype == ID)
		{
			ungetToken();
			if_st->condition_operand1 = primary();

			ttype = getToken();
			if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
			{
				if_st->condition_op = ttype;
				
				ttype = getToken();
				if (ttype == NUM || ttype == ID)
				{
					ungetToken();
					if_st->condition_operand2 = primary();
					if_st->false_branch = noop;
					if_st->true_branch = body(noop);
					return if_st;
				}
				else
					syntax_error("if_stmt. NUM, or ID expected");
			}
			else
				syntax_error("if_stmt. GREATER, LESS, or NOTEQUAL expected");

		}
		else
			syntax_error("if_stmt. NUM, or ID expected");
	}
	else
		syntax_error("if_stmt. IF expected");
	return NULL;
}

struct StatementNode* no_op_stmt()
{
	struct StatementNode* noop;

	noop = ALLOC(struct StatementNode);
	noop->next = NULL;
	noop->type = NOOP_STMT;

	return noop;
}

struct ValueNode* primary()
{
	struct ValueNode* prim;

	ttype = getToken();
	if (ttype == ID || ttype == NUM)
	{
		if (ttype == ID)
		{
			prim = var(token);
		}
		else
		{
			ungetToken();
			prim = number();
		}
		return prim;
	}
	else
		syntax_error("primary. ID, or NUM expected");
	return NULL;
}

struct IfStatement* while_stmt(struct StatementNode* noop, struct StatementNode* loop)
{
	struct IfStatement* whil;

	ttype = getToken();
	if (ttype == WHILE)
	{
		whil = ALLOC(struct IfStatement);
		whil->false_branch = noop;
		whil->condition_operand1 = primary();
		
		ttype = getToken();
		if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
		{
			whil->condition_op = ttype;
			whil->condition_operand2 = primary();

			whil->true_branch = body(loop);
			return whil;
		}
		else
			syntax_error("while_stmt. GREATER, LESS, or NOTEQUAL expected");
	}
	else
		syntax_error("while_stmt. WHILE expected");
	return NULL;
}

int findVar(char* search)
{
	int index, i;
	index = -1;

	for (i = 0; i < numVars; i++)
	{
		if (strcmp(varList[i]->name, search) == 0)
			index = i;
	}

	return index;
}

struct ValueNode* var(char* nam)
{
	int index;
	char *string;
	struct ValueNode* newVar;
	index = findVar(nam);

	if (index == -1)
	{
		string = malloc(sizeof(char[MAX_TOKEN_LENGTH]));
		newVar = ALLOC(struct ValueNode);
		newVar->value = 0;
		strcpy(string, nam);
		newVar->name = string;
		varList[numVars] = newVar;
		numVars++;
		return newVar;
	}
	else if (index >= 0)
	{
		return varList[index];
	}
	else
	{
		printf("Something messed up in var.");
		exit(0);
	}
	return NULL;
}

struct StatementNode* switch_stmt(struct StatementNode* noop)
{
	struct StatementNode* swit, *root, *hold;
	struct ValueNode* test;

	ttype = getToken();
	if (ttype == SWITCH)
	{
		swit = ALLOC(struct StatementNode);
		swit->type = IF_STMT;
		swit->next = noop;

		ttype = getToken();
		if (ttype == ID)
		{
			test = var(token);

			ttype = getToken();
			if (ttype == LBRACE)
			{
	
				ttype = getToken();
				if (ttype == CASE)
				{
					ungetToken();
					root = swit;
					root->next = noop;
					swit->if_stmt = case_stmt(noop, test);
					hold = swit;

					ttype = getToken();
					while (ttype == CASE)
					{
						ungetToken();
						hold = swit;
						swit = ALLOC(struct StatementNode);
						swit->type = IF_STMT;
						swit->if_stmt = case_stmt(noop, test);
						hold->if_stmt->true_branch = swit;
						ttype = getToken();
					}
					
					if (ttype == DEFAULT)
					{
						ttype = getToken();
						if (ttype == COLON)
						{
							swit->if_stmt->true_branch = body(noop);

							ttype = getToken();
							if (ttype == RBRACE)
								return root;
							else
								syntax_error("switch_stmt. RBRACE still expected after default");
			
						}
						else
							syntax_error("switch_stmt. COLON expected");
					}
					else if (ttype == RBRACE)
					{
						swit->if_stmt->true_branch = noop;
						return root;
					}
					else
						syntax_error("switch_stmt. RBRACE expected");
				}
				else
					syntax_error("switch_stmt. CASE expected");
			}
			else
				syntax_error("switch_stmt. LBRACE expected");
		}
		else
			syntax_error("switch_stmt. ID expected");
	}
	else
		syntax_error("switch_stmt. SWITCH expected");
	return NULL;
}

struct StatementNode* stmt_list(struct StatementNode* next)
{
	struct StatementNode* statement;
	struct StatementNode* tempNoOp;

	ttype = getToken();
	if (ttype == ID || ttype == PRINT)
	{
		ungetToken();
		statement = stmt();

		ttype = getToken();
		if (ttype == ID || ttype == PRINT || ttype == WHILE || ttype == IF || ttype == SWITCH)
		{
			ungetToken();
			statement->next = stmt_list(next);
			return statement;
		}
		else
		{
			ungetToken();
			statement->next = next;
			return statement;
		}
	}
	else if (ttype == WHILE || ttype == IF || ttype == SWITCH)
	{
		ungetToken();
		statement = stmt();
		tempNoOp = statement->next;
	
		ttype = getToken();
		if (ttype == ID || ttype == PRINT || ttype == WHILE || ttype == IF || ttype == SWITCH)
		{
			ungetToken();
			tempNoOp->next = stmt_list(next);
			return statement;
		}
		else
		{
			ungetToken();
			tempNoOp->next = next;
			return statement;
		}
	}
	
	return NULL;
}

void var_section()
{
	ttype = getToken();
	
	if (ttype == ID)
	{
		ungetToken();
		id_list();

		ttype = getToken();
		if (ttype != SEMICOLON)
			syntax_error("var_section. SEMICOLON expected");
	}
	else
		syntax_error("var_section. ID expected");
}

void id_list()
{
	ttype = getToken();

	if (ttype == ID)
	{
		var(token);

		ttype = getToken();
		if (ttype == COMMA)
		{
			id_list();
		}
		else
		{
			ungetToken();
		}
	}
	else
		ungetToken();
}

struct StatementNode* parse_generate_intermediate_representation()
{
	struct StatementNode* bod;
	var_section();
	bod = body(NULL);
	return bod;
}

// < "$(ProjectDir)tests/test_assignment_basic1.txt"