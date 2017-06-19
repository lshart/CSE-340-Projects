#ifndef __LEXER__H__
#define __LEXER__H__

// -------------------------------- token types --------------------------------
#define IF        1
#define WHILE     2
#define DO        3
#define THEN      4
#define PRINT     5
#define PLUS      6
#define MINUS     7
#define DIV       8
#define MULT      9
#define EQUAL     10
#define COLON     11
#define COMMA     12
#define SEMICOLON 13
#define LBRAC     14
#define RBRAC     15
#define LPAREN    16
#define RPAREN    17
#define NOTEQUAL  18
#define GREATER   19
#define LESS      20
#define LTEQ      21
#define GTEQ      22
#define DOT       23
#define NUM       24
#define ID        25
#define ERROR     26

// ----------- Global variables associated with the next input token -----------
#define MAX_TOKEN_LENGTH 200
extern char token[MAX_TOKEN_LENGTH];	// token string
extern int  tokenLength;				// token length
extern int  ttype;						// token type
extern int  line;						// current line number

// ------------------------------ Lexer functions ------------------------------
/*
 * Reads the next token from standard input and returns its type. The actual
 * value of the token is stored in the global variable token and the global
 * variable ttype will store the return value of the last call to getToken()
 */
int getToken();

/*
 * Sets a flag so that the next call to getToken() would return the last token
 * read from the input instead of reading a new token
 */
void ungetToken();

#endif //__LEXER__H__
