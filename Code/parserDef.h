/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/

#ifndef _PARSERDEF_H
#define _PARSERDEF_H

#include "lexerDef.h"

extern int line_count;
extern int buf_count;
extern int DFA_state;
extern int Printst;
extern int factor_nt;
extern int factor_t;
extern int no_of_terminals;
extern int SynxErr;
extern int LexErr;
extern int SemErr;

typedef struct st_node st_node;

typedef struct RHS RHS;
struct RHS{
    int check;
    char *value;
    RHS* next;
};


typedef struct LHS{
    char* value;
    int e;
    RHS* head;
}LHS;


typedef struct grammar{
    LHS* start;
    int size;
    int no_of_nt;
}grammar;


typedef struct First{
    char* value;
    char** first_list;
    int size_first_list;
    int e;
}First;


typedef struct Follow{
    char* value;
    char** follow_list;
    int size_follow_list;
    int d;
}Follow;


typedef struct FirstAndFollow{
    First* first;
    Follow* follow;
    int no_of_firsts;
    int no_of_follows;
}FirstAndFollow;

FirstAndFollow * fo;


typedef struct table{
    LHS* ** head; 
    int r;
    int c;
}table;

typedef struct type
{
	bool arr;
	int start_arr;
	int end_arr;
	char* node_type;
	int Line_No;
}type;


//structure for one symbol
typedef struct symbol symbol;
struct symbol
{
	char* var_name;
	char* type;
	int start_scope;//start - line where the variable is declared; 
	bool func;
	int size;
	int offset;
	bool arr;
	bool definition;
	bool declare;
	bool dec_used;
	bool use;//change
	//if the symbol is a part of the input list or output list of a function
	bool input;
	bool output;
	int def_line_no;
	int start_arr;
	int end_arr;
	bool start_arr_dec;
	bool end_arr_dec;
	symbol* next;
	symbol* next_input_output;//linked list of input/output
	bool mark;
	bool forvar;
	st_node* symboltable;
	char* arr_start;
	char* arr_end;
	symbol* arr_start_sym;
	symbol* arr_end_sym;
};


typedef struct ast_node ast_node;
struct ast_node
{
	ast_node* parent;
	ast_node* child;
	ast_node* next;
	ast_node* prev;
	int childsize;
	union name{
		char nt_name[30];
		Lexeme* l;
	}name;
	int tag;
	//pointer to symbol in symbol table if node is ID
	symbol* symbol;
	type* ty;
};


typedef struct node node;
struct node
{
	node* parent;
	char* value;
	Lexeme* lex;
	int check;
	int done;
	node** children;
	int numofchild;
	ast_node* addr;
};


typedef struct parseTree{
	node* root;
}parseTree;


parseTree* p;

char ** nonterminals;
char** terminals;
grammar gram;


typedef struct hash_nt hash_nt;
struct hash_nt
{
	char* value;
	int index;
	hash_nt* next;
};

typedef struct hash_t hash_t;

struct hash_t
{
	char* value;
	int index;
	hash_t* next;
};


hash_nt** hash_nt_ptr;
hash_t** hash_t_ptr;

FILE* fpt;
FILE* asfile;


typedef struct stack_node{ 
	char* value;
	int check; 
    	node* ptr;
    	struct stack_node* link; 
}stack_node; 


stack_node* top;

table T;

FirstAndFollow F;


#endif

