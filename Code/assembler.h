/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/

#include "symbolTable.h"

#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include "assemblerDef.h"

void initializeLables();

void CodeGenStart(ast_tree* tree, st_node* root, char* file);

void CodeGen(ast_node* root,inherited f, st_node* st_root);

void fill_ret_vars(ast_node* input_list, ast_node* insert_list);

void intialise_inp_vars(ast_node* input_list, ast_node* insert_list);

void PrintTemplateStart();

void PrintTemplateEnd();

#endif