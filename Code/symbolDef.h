/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#ifndef _SYMBOLDEF_H
#define _SYMBOLDEF_H

#include "astDef.h"

#define SIZEOF_ST_HASH_TABLE 10

int scope;
int offset;
extern int SemErr;
symbol* sym_list;
bool driver_test;
extern int symind;

typedef struct st_hashtable{
	int size;
	struct symbol* arr[SIZEOF_ST_HASH_TABLE];
}st_hashtable;


typedef struct flag flag;
struct flag
{
	bool dec;
	bool ip_plist;
	bool op_plist;
	bool mod;// whether the last insertion was input_plist then dont change the scope
	bool mod_dec;
};


//one symbol table
struct st_node
{
	st_hashtable* head;
	symbol* input_head;
	symbol* output_head;
	char* func_name;
	int scope;
	st_node* parent;
	int end_scope;
	st_node* next;	
	st_node* child;
	int level;
	int maxoffset;
	ast_node* module_ptr;
};


typedef struct inherited inherited;
struct inherited
{
	int cond_offset;
	int switch_label;
};
#endif
