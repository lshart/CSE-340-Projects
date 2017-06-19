#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

typedef struct node
{
	char token_node[MAX_TOKEN_LENGTH];
	int  ttype_node;
	int  line_node;
	struct node *next;
}TListNode;

void addToken(TListNode **head, TListNode *newNode);
void deleteList(TListNode *head);
void printToken(TListNode *thisNode);
void printInReverse(TListNode *head);
TListNode *newTListNode(char token[MAX_TOKEN_LENGTH], int ttype, int line);

int main()
{
	TListNode *head = NULL;

	while (getToken() != EOF)
	{
		if (ttype == NUM)
		{
			addToken(&head, newTListNode(token, ttype, line));
		}
		else if (ttype == ID && (strcmp("cse340", token) == 0 || strcmp("programming", token) == 0 || strcmp("language", token) == 0))
		{
			addToken(&head, newTListNode(token, ttype, line));
		}
	}

	printInReverse(head);

	deleteList(head);
}

TListNode *newTListNode(char newToken[MAX_TOKEN_LENGTH], int newTtype, int newLine)
{
	TListNode* newNode = malloc(sizeof(TListNode));
	strcpy(newNode->token_node, newToken);
	newNode->ttype_node = newTtype;
	newNode->line_node = newLine;
	newNode->next = NULL;

	return newNode;
}

void addToken(TListNode **head, TListNode *newNode)
{
	TListNode *iterator = *head;

	if (iterator == NULL)
	{
		*head = newNode;
	}
	else
	{
		while (iterator->next != NULL)
		{
			iterator = iterator->next;
		}
		iterator->next = newNode;
	}
}

void printToken(TListNode *thisNode)
{
	printf("%i", thisNode->line_node);
	if (thisNode->ttype_node == NUM)
		printf("%s", " NUM ");
	else if (thisNode->ttype_node == ID)
		printf("%s", " ID ");
	printf("%s", thisNode->token_node);
	printf("%c", '\n');
}

void printInReverse(TListNode *thisNode)
{
	if (thisNode->next != NULL)
		printInReverse(thisNode->next);
	printToken(thisNode);
}

void deleteList(TListNode *head)
{
	if (head != NULL)
	{
		TListNode *iterator = head;
		TListNode *tail;
		while (iterator->next != NULL)
		{
			tail = iterator;
			iterator = iterator->next;
			free(tail);
		}
	}
}