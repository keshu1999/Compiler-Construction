/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#ifndef _AST_H
#define _AST_H

#include "astDef.h"

static inline void* add_hash_table_ast(char * p,struct ast_h_table* ht);

struct ast_h_table* ast_initialization();

int ast_find_token(char* pattern);

ast_stack_node* ast_top;

ast_node* make_node(int argc, ast_node** argv, char* name);

ast_node* make_leaf(Lexeme* lex);

//AST Stack functions

int ast_isEmpty();

void ast_push(node* ptr, int ind);

ast_stack_node* ast_peek();

void ast_pop();

void ast_increment();

ast_tree* ast_creation(parseTree *p);

void PrintAST(ast_node *root);

void CountParseTree(node *root);

void CountAST(ast_node *root);

#endif
