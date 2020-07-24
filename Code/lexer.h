/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/

#ifndef _LEXER_H
#define _LEXER_H

#include "lexerDef.h"

static inline void* hash_table(char* p, char* t, struct h_table* ht);

struct h_table* initialize_hash_table();

char* find_token(char* pattern);

static inline double get_value_double(char* input);

char* getStream(FILE *fp);

Lexeme* getNextToken();

void Token_Print(FILE* fp);

void Toktime(FILE *fp);

void removeComments(FILE *testCaseFile);

#endif
