/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "parserDef.h"
#ifndef _PARSER_H
#define _PARSER_H

int last_occurence(char* s,grammar G);

int first_occurence(char* s,grammar G);

int checkntort(char * ch, char ** strs,grammar G);

void find_nt(grammar G);

void populate_nt(grammar *G);

int calculate_hash(char* s,int factor);

void fill_hash_nt_table();

void fill_hash_t_table();

bool already_present(char** arr, char* str,int size);

int rank(char* nt);

int terminal_rank(char* s);

int fill_terminal(char* s,int size);

void find_firsts(FirstAndFollow F, int i,grammar G, int it);

void find_follow(FirstAndFollow F, int i, RHS* start, LHS l,grammar G);

FirstAndFollow ComputeFirstAndFollowSets(grammar G, FirstAndFollow  F,int it);

table createParseTable(FirstAndFollow F, table T);

node* parse_tree(LHS* lhs , node* n, Lexeme* lex);

void parseInputSourceCode(FILE * fp1, table T);

void printParseTree(node* n);

void parser();

void printExitMsg();

void print_First();

void print_Follow();

void freetree(node *n);

//STACK FUNCTIONS

void push(char* value , int check, node* ptr);

int isEmpty();

stack_node* peek();

void pop();

void display();


#endif
