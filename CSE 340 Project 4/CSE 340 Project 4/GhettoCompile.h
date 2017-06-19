#ifndef _GHETTO_COMPILE_
#define _GHETTO_COMPILE_

struct AssignmentStatement* assn_stmt();
struct PrintStatement* print_stmt();
struct IfStatement* if_stmt(struct StatementNode* noop);
struct IfStatement* while_stmt(struct StatementNode* noop, struct StatementNode* loop);
struct IfStatement* case_stmt(struct StatementNode* noop, struct ValueNode* test);
struct StatementNode* switch_stmt(struct StatementNode* noop);
struct StatementNode* no_op_stmt();
struct StatementNode* body(struct StatementNode* next);
struct StatementNode* stmt_list(struct StatementNode* next);
struct StatementNode* stmt();
struct ValueNode* primary();
struct ValueNode* number();
struct ValueNode* var(char* name);
void var_section();
void id_list();

#endif