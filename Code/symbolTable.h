/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "symbolDef.h"
#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

static inline int add_hash_table_st(symbol * sym,struct st_hashtable* sht);

void insert_into_hashtable(symbol* sym_list, st_node* st_current);

symbol* st_find_token(char* sym,st_node* st_current);

symbol* st_lookup(st_node* current , char* search);

st_node* insert_child(st_node* st_current,int scope);

void fill_input_output(st_node* st_current , symbol* temp, int check);

st_node* lookup_function_main_scope_table(char* ID , st_node* current, bool ipop);

void create_sym_tree(ast_node* p,st_node* st_current, flag* f);

void print_activation_record(st_node* current);

st_node* create_st(ast_tree* p);

void * print_st( st_node* current );

void* printSymbolTable(st_node* root);

void * print_arr(st_node* current);

void* printArray(st_node* root);

int arr_check(symbol* s1 , symbol* s2);

int for_lookahead(ast_node* current , char* forid);

void Semantic_Check(ast_node* p,st_node* st_current, flag* f);

bool is_same_type(type* ty1, type* ty2);

void type_check(ast_node* p);

void Type_Checking(ast_tree* p);


#endif
