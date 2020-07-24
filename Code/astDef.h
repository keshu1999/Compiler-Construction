/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#ifndef _ASTDEF_H
#define _ASTDEF_H

#include "parserDef.h"

#define SIZEOF_AST_HASH_TABLE 29

extern int numAST;
extern int numParseTree;

struct ast_h_table* ast_ht;

struct ast_h_table{
	int size;
	struct ast_keywords* arr[SIZEOF_AST_HASH_TABLE];
};


struct ast_keywords{
	char* pattern;
	struct ast_keywords* next;
};


typedef struct ast_tree{	
	ast_node* root;
}ast_tree;

ast_tree* asttreee;


typedef struct stack
{
	int index;
	node* pnode;
	struct stack* down;

}ast_stack_node;

#endif


