/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "symbolTable.h"


int track_func_scope = 0;									//remembers the scope number of the current function being traversed 

static inline int add_hash_table_st(symbol * sym,struct st_hashtable* sht)
{
	sym->mark = 0;
	int length_of_string=0;
	int len=0;
	char* p=sym->var_name;
	while (p[length_of_string])
		length_of_string++;
	size_t siz=length_of_string+1;
	char * pat=malloc(siz*sizeof(char));
	char* pat1=pat;
	while(*p)
	{
		*pat1=*p;
		p++;
		pat1++;
	}
	*pat1='\0';
	symbol * traverse;
	int a=0;
	for (int i=0;i<length_of_string;i++)
		a=a+pat[i];
	int index_of_st_hashtable=(a*a+5*a)%SIZEOF_ST_HASH_TABLE;
	sht->size++;
	if(sht->arr[index_of_st_hashtable]==NULL)
	{
		sht->arr[index_of_st_hashtable]=sym;
		sym->next=NULL;
	}
	else
	{	
		traverse=sht->arr[index_of_st_hashtable];
		
		while(traverse->next!=NULL)
		{
			if(!strcmp(traverse->var_name,sym->var_name) && traverse->declare==1 && traverse->definition!=1 && sym->definition==1)
			{	
				traverse->definition=1 ;
				traverse->def_line_no=sym->start_scope;
				if(!traverse->dec_used)
				{
					printf(MAGENTA"WARNING: "RESET"Redundant declaration of function %s before its first use "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Module %s was declared and defined before its use\n\n" ,traverse->var_name, traverse->def_line_no, traverse->var_name);
					SemErr++;
				}
				return 1;
			}
			if(!strcmp(traverse->var_name,sym->var_name) && traverse->declare==1 && traverse->definition==1 && sym->definition==1)
			{	
				printf(RED"ERROR: "RESET"Redefinition of Module %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Previous declaration of %s was in Line Number: %d\n\n",sym->var_name, sym->start_scope, sym->var_name, traverse->start_scope);
				SemErr++;
				return 0;
			}
			if(!strcmp(traverse->var_name,sym->var_name) )
			{
				printf(RED"ERROR: "RESET"Redeclaration of %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Previous declaration of %s was in Line Number: %d\n\n",sym->var_name, sym->start_scope, sym->var_name,traverse->start_scope);
				SemErr++;
				return 1;
			}
			traverse=traverse->next;
		}
		if(!strcmp(traverse->var_name,sym->var_name) && traverse->declare==1&& traverse->definition!=1  && sym->definition==1)
		{	
			traverse->definition=1;
			traverse->def_line_no=sym->start_scope;
				if(!traverse->dec_used)
				{
					printf(MAGENTA"WARNING: "RESET"Redundant declaration of function %s before its first use "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Module %s was declared and defined before its use\n\n" ,traverse->var_name, traverse->def_line_no, traverse->var_name);
					SemErr++;
				}
			return 1;
		}
		if(!strcmp(traverse->var_name,sym->var_name) && traverse->declare==1 && traverse->definition==1 && sym->definition==1)
		{	
			printf(RED"ERROR: "RESET"Redefinition of Module %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Previous declaration of %s was in Line Number: %d\n\n",sym->var_name, sym->start_scope, sym->var_name,traverse->start_scope);
			SemErr++;
			return 0;
		}
		if(!strcmp(traverse->var_name,sym->var_name) )
		{
			printf(RED"ERROR: "RESET"Redeclaration of %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Previous declaration of %s was in Line Number: %d\n\n",sym->var_name, sym->start_scope, sym->var_name,traverse->start_scope);
			SemErr++;
			return 1;
		}
		traverse->next=sym;
		sym->next=NULL;
	}
	return 1;
}


void insert_into_hashtable(symbol* sym_list, st_node* st_current)
{
	symbol* temp1,* temp2;
	temp1=sym_list;
	if(temp1==NULL)
		printf(RED"Error: Symlist is empty\n"RESET);
	while(temp1!=NULL)
	{	
		temp2=temp1->next;
		add_hash_table_st(temp1,st_current->head);
		temp1->symboltable = st_current;
		temp1=temp2;
	}
}


symbol* st_find_token(char* sym, st_node* st_current)
{	
	char* pattern=sym;
	int a=0;
	int length_of_string=0;
	char *p=pattern;
	while(*pattern)
	{ 
		length_of_string++;
		*pattern++;
	}
	for(int i=0;i<length_of_string;i++)
		a=a+p[i];
	int index_of_htable=(a*a+5*a)%SIZEOF_ST_HASH_TABLE;
	symbol* temp1=st_current->head->arr[index_of_htable];
	char * x;
	char* y;
	while(temp1!=NULL&&temp1->next!=NULL)
	{	
		x=temp1->var_name;
		y=p;
		while(*x)
		{
			if(*x!=*y)
				break;
			x++;
			y++;
		}
		if(!(*(const unsigned char*)x-*(const unsigned char*)y))
			return temp1;
		else
			temp1=temp1->next;
	}
	if(temp1!=NULL)
	{
		x=temp1->var_name;
		y=p;
		while(*x)
		{		
			if(*x!=*y)
				break;
			x++;
			y++;
		}
		if(!(*x-*y))
			return temp1;
		else
			return NULL;
	}
	else
		return NULL;
	
}


symbol* st_lookup(st_node* current , char* search)
{
	symbol* temp;
	do
	{
		if((temp = st_find_token(search, current)) != NULL)
		{ 
			return temp;
			break;
		}
		current = current->parent;
	}while(current != NULL);

	return NULL;
}


st_node* insert_child(st_node* st_current,int scope)
{
	st_node* current;	
	if(st_current->child==NULL)
	{
		st_current->child=malloc(sizeof(st_node));
		current=st_current->child;		
		current->head=malloc(sizeof(st_hashtable));
		for (int i=0;i<SIZEOF_ST_HASH_TABLE;i++)
			current->head->arr[i]=NULL;
		current->scope=scope;
		current->level=st_current->level+1;
		current->parent=st_current;
		current->next=NULL;
		current->input_head = NULL;
		current->output_head = NULL;
		current->child = NULL;
	}
	else
	{
		current=st_current->child;
		while(current->next!=NULL)
			current=current->next;
		current->next=malloc(sizeof(st_node));
		current=current->next;
		current->head=malloc(sizeof(st_hashtable));
		for (int i=0;i<SIZEOF_ST_HASH_TABLE;i++)
			current->head->arr[i]=NULL;
		current->scope=scope;
		current->level=st_current->level+1;
		current->parent=st_current;
		current->next=NULL;
		current->input_head = NULL;
		current->output_head = NULL;
		current->child = NULL;
	}
	return current;	
}


void fill_input_output(st_node* st_current , symbol* temp, int check)
{
	symbol* lite = NULL;
	if(check == 1)
	{	
		if(st_current != NULL && st_current->input_head != NULL)
			lite = st_current->input_head;
	}
	else
	{ 
		if(st_current != NULL && st_current->output_head != NULL)
			lite = st_current->output_head;
	}
	if(lite == NULL)
	{	
		if(check == 1)
			st_current->input_head = temp;
		else 
			st_current->output_head = temp;
		return;
	}
	while(lite->next_input_output != NULL)
		lite = lite->next_input_output;

	lite->next_input_output = temp;
}


st_node* lookup_function_main_scope_table(char* ID , st_node* current, bool ipop)
{
	while(current->parent != NULL)
		current = current->parent;

	if(st_lookup(current , ID) == NULL)
		return NULL;
	if(ipop == 1)
	{
		current = current->child;
		while(current != NULL)
		{
			if(!strcmp(current->func_name, ID))
				return current;
			current = current->next;
		}
		return NULL;
	}
	else
	{
		current = current->child;
		while(current != NULL)
		{
			if(!strcmp(current->func_name, ID))
				return current->child;
			current = current->next;
		}
		return NULL;
	}
}


void create_sym_tree(ast_node* p,st_node* st_current, flag* f)
{
	if(p==NULL)
		return ;	
	ast_node* current=p;
	bool new_scope=0;
	if(current->tag==0 && !strcmp(current->name.nt_name,"module"))
	{	
		f->mod=1;
		scope++;
		offset = 0;
		symbol* temp=malloc(sizeof(symbol));
		temp->func=1;
		temp->definition=1;
		temp->declare=0;
		temp->dec_used=0;
		temp->start_arr=0;
		temp->end_arr=0;
		temp->start_arr_dec=0;
		temp->end_arr_dec=0;
		temp->start_scope=current->child->name.l->Line_No;
		temp->var_name=current->child->name.l->lexeme;
		temp->type=malloc(sizeof(char)*7);
		strcpy(temp->type,"MODULE");	
		temp->next=NULL;
		temp->offset = -1;
		temp->size = -1;
		insert_into_hashtable(temp,st_current);
		st_current=insert_child(st_current,scope);
		st_current->module_ptr = current;
		st_current->func_name=current->child->name.l->lexeme;		
		st_current=insert_child(st_current,scope);
		st_current->func_name=st_current->parent->func_name;
		st_current=st_current->parent;
		new_scope=1;
	}	
	if(current->tag==0 && !strcmp(current->name.nt_name,"program") && current->child->tag==1)
	{	
		symbol* temp=malloc(sizeof(symbol));
		temp->func=1;
		temp->definition=0;
		temp->declare=1;
		temp->dec_used=0;
		temp->var_name=current->child->name.l->lexeme;
		temp->start_scope=current->child->name.l->Line_No;
		temp->next=NULL;
		insert_into_hashtable(temp,st_current);
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDeclarations"))
		f->mod_dec=1;	
	if(current->tag==0 && (!strcmp(current->name.nt_name,"moduleDef")  && f->mod==0))
	{	
		scope++;
		track_func_scope = scope;
		offset = 0;	
		symbol* temp1=malloc(sizeof(symbol));
		temp1->func=1;
		temp1->definition=1;
		temp1->declare=0;
		temp1->dec_used=0;
		temp1->start_arr=0;
		temp1->end_arr=0;
		temp1->start_arr_dec=0;
		temp1->end_arr_dec=0;
		temp1->start_scope=current->child->name.l->Line_No;
		temp1->var_name=malloc(sizeof(char)*7);
		strcpy(temp1->var_name , "driver");
		temp1->type=malloc(sizeof(char)*7);
		strcpy(temp1->type,"MODULE");	
		temp1->next=NULL;
		temp1->offset = -1;
		temp1->size = -1;
		insert_into_hashtable(temp1 , st_current);
		st_current=insert_child(st_current,scope);
		st_current->level = 1;
		st_current->func_name=malloc(sizeof(char)*8);
		strcpy(st_current->func_name,"driver");
		ast_node* temp = current->child;
		while(temp!=NULL)
		{
			if(temp->tag==1 && !strcmp(temp->name.l->TOKEN,"END"))
				st_current->end_scope=temp->name.l->Line_No;				
			temp=temp->next;		
		}	
		new_scope=1;
	}
	if(!strcmp(current->name.nt_name,"conditionalStmt")   )
	{
		scope++;		
		st_current=insert_child(st_current,scope);
		st_current->func_name=st_current->parent->func_name;
		ast_node* temp = current->child;
		while(temp!=NULL)
		{
			if(temp->tag==1 && !strcmp(temp->name.l->TOKEN,"END"))
				st_current->end_scope=temp->name.l->Line_No;				
			temp=temp->next;		
		}	
		new_scope=1;
	}
	if(!strcmp(current->name.nt_name,"iterativeStmt"))
	{	
		symbol * arr ;
		scope++;
		
		if(!strcmp(current->child->name.l->lexeme,"for"))
		{
			char* name = current->child->next->name.l->lexeme;
			arr= st_lookup( st_current,name);
			if(current->child->next->next->child->name.l->Value.val >= current->child->next->next->child->next->name.l->Value.val)
			{	
				printf(RED"ERROR: "RESET"For Loop indices ranging from higher value: %s to lower value: %s "LINE"in Line Number: %d"RESET"\n\n",current->child->next->next->child->name.l->lexeme,current->child->next->next->child->next->name.l->lexeme,current->child->name.l->Line_No);
				SemErr++;
			}	
			st_current=insert_child(st_current,scope);
		}
			
		else
		{
			if(current->child->next->next->next->childsize==0)
			{
				printf(MAGENTA"WARNING: "RESET"Expected expression/statement(s) in the While Loop "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"The While Loop will never terminate\n\n",current->child->name.l->Line_No);
				SemErr++;
			}
			st_current=insert_child(st_current,scope);
		}

		st_current->func_name=st_current->parent->func_name;
		ast_node* temp = current->child;
		while(temp!=NULL)
		{
			if(temp->tag==1 && !strcmp(temp->name.l->TOKEN,"END"))
				st_current->end_scope=temp->name.l->Line_No;				
			temp=temp->next;		
		}	
		new_scope=1;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDef")&&  f->mod==1)
	{
		f->mod=0;
		ast_node* temp = current->child;
		if(st_current->parent->parent == NULL)
		{
			st_current=st_current->child;					
			scope++;
			track_func_scope = scope;	
			st_current->scope=scope;
		}

		while(temp!=NULL)
		{
			if(temp->tag==1 && !strcmp(temp->name.l->TOKEN,"END"))
			{
				st_current->end_scope=temp->name.l->Line_No;				
				if(st_current->parent->parent != NULL)
				{
					st_current->parent->end_scope=temp->name.l->Line_No;
				}				
			}
			temp=temp->next;		
		}	
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"input_plist"))
		f->ip_plist=1;

	if(current->tag==0 && !strcmp(current->name.nt_name,"output_plist"))
	{
		f->op_plist=1;
		st_current = st_current->child;
		scope++;
		track_func_scope = scope;
		st_current->scope=scope;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"declareStmt"))
		f->dec=1;	
	
	if(f->mod_dec==1 && (current->tag==1) && !strcmp(current->name.l->TOKEN,"ID"))
	{
		symbol* temp=malloc(sizeof(symbol));
		temp->func=1;
		temp->definition=0;
		temp->declare=1;
		temp->dec_used=0;
		temp->var_name=current->name.l->lexeme;
		temp->start_scope=current->name.l->Line_No;
		temp->type=malloc(sizeof(char)*7);
		strcpy(temp->type,"MODULE");	
		temp->next=NULL;
		temp->offset = -1;
		temp->size = -1;
		insert_into_hashtable(temp,st_current);
	}

	if(current->tag==1 && f->dec==1 && !strcmp(current->name.l->TOKEN,"ID"))  
	{
		symbol* temp;		
		if(sym_list==NULL)
		{
			sym_list=malloc(sizeof(symbol));
			temp=sym_list;
			temp->next=NULL;
		}
		else
		{
			temp=sym_list;			
			while(temp->next!=NULL)
				temp=temp->next;
			temp->next=malloc(sizeof(symbol));
			temp=temp->next;
			temp->next=NULL;
		}
		
		temp-> var_name=current->name.l->lexeme;
		temp->start_scope=current->name.l->Line_No;
		temp->definition=0;
		temp->declare=0;
		temp->dec_used=0;
		temp-> func=0;
		temp ->arr=0;
		temp -> start_arr=0;
		temp->end_arr=0;
		temp->start_arr_dec=0;	
		temp->end_arr_dec=0;
		current->symbol = temp;
	}

	if(current->tag==1 && f->dec==1 && (!strcmp(current->name.l->TOKEN,"INTEGER")||!strcmp(current->name.l->TOKEN,"REAL")||!strcmp(current->name.l->TOKEN,"BOOLEAN")))
	{
		if(current->tag==1)
		{
			symbol* temp=sym_list;			
			if(sym_list!=NULL)
			{
				while(temp!=NULL)
				{
					temp->type=current->name.l->TOKEN;
					if(!strcmp(temp->type,"INTEGER"))
						temp->size=4;
					else if(!strcmp(temp->type,"REAL"))
						temp->size=8;
					else if(!strcmp(temp->type,"BOOLEAN"))
						temp->size=1;
					offset+=temp->size;
					temp->offset=offset;
					temp->arr=0;
					temp=temp->next;			
				}			
			}
			insert_into_hashtable(sym_list,st_current);
			sym_list=NULL;	
			f->dec=0;
		}	
	}

	if(current->tag==0 && f->dec==1 && f->ip_plist==0 && f->op_plist==0 && !strcmp(current->name.nt_name,"dataType"))
	{
		if(current->child->tag==0)
		{
			if(sym_list!=NULL)
			{			
				symbol* temp=sym_list;			
				while(temp!=NULL)
				{
					temp->type=current->child->next->name.l->TOKEN;
					if(!strcmp(temp->type,"INTEGER"))
						temp->size=4;
					else if(!strcmp(temp->type,"REAL"))
						temp->size=8;
					else if(!strcmp(temp->type,"BOOLEAN"))
						temp->size=1;
					temp->arr=1;
					if(!strcmp(current->child->child->name.l->TOKEN,"NUM"))
					{
						temp->start_arr=atoi(current->child->child->name.l->lexeme);
						temp->start_arr_dec=1;
					}
					if(!strcmp(current->child->child->name.l->TOKEN,"ID"))
					{
						temp->arr_start=current->child->child->name.l->lexeme;
						temp->arr_start_sym=st_lookup(st_current , temp->arr_start);
						temp->start_arr_dec=0;
					}	
					if(!strcmp(current->child->child->next->name.l->TOKEN,"NUM"))
					{
						temp->end_arr=atoi(current->child->child->next->name.l->lexeme);
						temp->end_arr_dec=1;
					}
					if(!strcmp(current->child->child->next->name.l->TOKEN,"ID"))
					{
						temp->arr_end=current->child->child->next->name.l->lexeme;
						temp->arr_end_sym=st_lookup(st_current , temp->arr_end);
						temp->end_arr_dec=0;
					}
					if(!strcmp(current->child->child->name.l->TOKEN,"NUM") && !strcmp(current->child->child->next->name.l->TOKEN,"NUM"))
					{	
						temp->size=(temp->size)*(temp->end_arr-temp->start_arr+1) + 12;
						if(temp->start_arr>temp->end_arr)
						{
							printf(RED"ERROR: "RESET"Array indices range from higher value: %d to lower value: %d "LINE"in Line Number: %d"RESET"\n\n",temp->start_arr,temp->end_arr,current->child->child->name.l->Line_No);
							SemErr++;
						}
					}
					else
						temp->size = 12;

					offset+=temp->size;
					temp->offset=offset;
					temp=temp->next;
				}
			}
			insert_into_hashtable(sym_list,st_current);
			sym_list=NULL;	
			f->dec=0;
		}	
	}
	if(current->tag==0 && f->ip_plist==1 && f->dec==0 && (!strcmp(current->name.nt_name,"iplist")||!strcmp(current->name.nt_name,"input_plist")))
	{
		if(current->child->tag==1 && !strcmp(current->child->name.l->TOKEN,"ID"))
		{
			symbol *temp=malloc(sizeof(symbol));
			temp-> var_name=current->child->name.l->lexeme;
			temp->start_scope=current->child->name.l->Line_No;
			temp->definition=0;
			temp->declare=0;
			temp->dec_used=0;
			temp-> func=0;
			temp ->arr=0;
			temp -> start_arr=0;
			temp->end_arr=0;
			temp->start_arr_dec=0;
			temp->end_arr_dec=0;
			temp->next=NULL;
			temp->next_input_output = NULL;
			temp->input = 1;
			fill_input_output(st_current , temp, 1);
			ast_node* curr=current->child->next;				
			if(current->child->next->tag==1)
			{
				temp->type=curr->name.l->TOKEN;
				if(!strcmp(temp->type,"INTEGER"))
					temp->size=4;
				else if(!strcmp(temp->type,"REAL"))
					temp->size=8;
				else if(!strcmp(temp->type,"BOOLEAN"))
					temp->size=1;
				offset+=temp->size;		
				temp->offset=offset;
			}
			else if(current->child->next->tag==0)
			{
				temp->type=curr->child->next->name.l->TOKEN;
				if(!strcmp(temp->type,"INTEGER"))
					temp->size=4;
				else if(!strcmp(temp->type,"REAL"))
					temp->size=8;
				else if(!strcmp(temp->type,"BOOLEAN"))
					temp->size=1;
				temp->arr=1;
				if(!strcmp(curr->child->child->name.l->TOKEN,"NUM"))
				{
					temp->start_arr=curr->child->child->name.l->Value.val;
					temp->start_arr_dec=1;
				}	
				if(!strcmp(curr->child->child->name.l->TOKEN,"ID"))
				{
					temp->arr_start=curr->child->child->name.l->lexeme;
					temp->arr_start_sym=st_lookup(st_current , temp->arr_start);
					temp->start_arr_dec=0;
				}
				if(!strcmp(curr->child->child->next->name.l->TOKEN,"NUM"))
				{
					temp->end_arr=curr->child->child->next->name.l->Value.val;
					temp->end_arr_dec=1;
				}
				
				if(!strcmp(curr->child->child->next->name.l->TOKEN,"ID"))
				{
						temp->arr_end=curr->child->child->next->name.l->lexeme;
						temp->arr_end_sym=st_lookup(st_current , temp->arr_end);
						temp->end_arr_dec=0;
				}
				
				if(!strcmp(curr->child->child->name.l->TOKEN,"NUM") && !strcmp(curr->child->child->next->name.l->TOKEN,"NUM"))
				{
					temp->size=(temp->size)*(temp->end_arr-temp->start_arr+1) + 12;
					if(temp->start_arr>temp->end_arr)
					{
						printf(RED"ERROR: "RESET"Array indices range from higher value: %d to lower value: %d "LINE"in Line Number: %d"RESET"\n\n",temp->start_arr,temp->end_arr,current->child->name.l->Line_No);
						SemErr++;
					}
				}
				else
					temp->size = 12;				
				offset+=temp->size;			
				temp->offset=offset;
			}
			insert_into_hashtable(temp,st_current);
		}
	}
	if(current->tag==0 && f->op_plist==1 && f->dec==0 && (!strcmp(current->name.nt_name,"oplist")||!strcmp(current->name.nt_name,"output_plist")))
	{
		if(current->child->tag==1 && !strcmp(current->child->name.l->TOKEN,"ID"))
		{
			symbol *temp=malloc(sizeof(symbol));
			temp-> var_name=current->child->name.l->lexeme;
			temp->start_scope=current->child->name.l->Line_No;
			temp->definition=0;
			temp->declare=0;
			temp->dec_used=0;
			temp-> func=0;
			temp ->arr=0;
			temp -> start_arr=0;
			temp->end_arr=0;
			temp->start_arr_dec=0;
			temp->end_arr_dec=0;
			temp->next=NULL;
			temp->next_input_output = NULL;
			temp->output = 1;
			fill_input_output(st_current , temp, 2);
			ast_node* curr=current->child->next;				
			temp->type=curr->name.l->TOKEN;
			if(!strcmp(temp->type,"INTEGER"))
				temp->size=4;
			else if(!strcmp(temp->type,"REAL"))
				temp->size=8;
			else if(!strcmp(temp->type,"BOOLEAN"))
				temp->size=1;
			offset+=temp->size;		
			temp->offset=offset;
			insert_into_hashtable(temp,st_current);
		}
	}

	if(current->tag==0 && (!strcmp(current->name.nt_name,"moduleReuseStmt")))
	{	
		char * search;
		int line;
		if(current->childsize != 2 )
		{
			search=current->child->next->name.l->lexeme;
			line=current->child->next->name.l->Line_No;
		}
		else if(current->childsize == 2)
		{
			search=current->child->name.l->lexeme;
			line=current->child->name.l->Line_No;
		}
		st_node * temp_raj=st_current;
		while(temp_raj->parent != NULL)
			temp_raj =temp_raj->parent;
		 
		if (temp_raj!=NULL)
		{	
			symbol * arr = st_find_token(search, temp_raj);
			if(arr!=NULL)
				arr->use=1;
			else;
		}
	}

	if(current->tag==1 && (!strcmp(current->name.l->TOKEN,"ID")))
	{
		symbol* temporary;
		if((temporary = st_lookup(st_current , current->name.l->lexeme)) != NULL )
		{	if(f->dec==0)
				current->symbol = temporary;
			if(temporary->func==1)
			{
				if(!(f->mod_dec==1 ||(current->parent->tag==0 && !strcmp(current->parent->name.nt_name,"program") && current->parent->child->tag==1) || (current->parent->tag==0 && !strcmp(current->parent->name.nt_name,"module")))) 
					temporary->dec_used=1;
			}
		}
		else  if(f->ip_plist==1)
			current->symbol=NULL;
		else if(f->dec==0 )
		{	
			printf(RED"ERROR: "RESET"%s undeclared "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Each Undeclared Variable/Module is reported for each Scope it appears in\n\n" , current->name.l->lexeme,current->name.l->Line_No);
			current->symbol=NULL;
			SemErr++;
		}
	}
	
	//Check if non terminal is assignment statement or getval
	if((current->tag==0 && (!strcmp(current->name.nt_name,"assignmentStmt"))) || (current->tag == 1 && (!strcmp(current->name.l->TOKEN,"GET_VALUE"))))
	{
		st_node* temp_st = st_current;
		bool local = 0;
		char* search = current->tag==0?current->child->name.l->lexeme:current->next->name.l->lexeme;  
		while(track_func_scope != temp_st->scope)
		{
			if(st_find_token(search, temp_st) != NULL)
				local = 1;
			temp_st = temp_st->parent;
		}
		if(local == 0)
		{
			symbol* temp = NULL;
			if(temp_st->output_head != NULL)
				temp = temp_st->output_head;

			while(temp != NULL)
			{
				if(!strcmp(search, temp->var_name))
				{
					temp->mark = 1;
					break;
				}
				temp = temp->next_input_output;
			}
		}
	}

	if(current->tag == 1 && !strcmp(current->name.l->TOKEN,"END") && st_current->scope == track_func_scope)
	{	 	
		symbol* temp = NULL;
		if(st_current->output_head != NULL)
			temp = st_current->output_head;

		while(temp != NULL)
		{
			if(temp->mark == 0)
			{
				printf(MAGENTA"WARNING: "RESET"Output Variable %s declared "LINE"in Line Number: %d"RESET" has not been assigned any value\n\t"CYAN"NOTE: "RESET"Variable may take Garbage values to produce Incorrect result\n\n" , temp->var_name , temp->start_scope);
				SemErr++;
			}
			temp = temp->next_input_output;
		}
		st_current->maxoffset = offset;
	}

	ast_node* child= p->child;
	while(child!=NULL)
	{
		create_sym_tree(child,st_current,f);
		child = child->next;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"module"))
	{
		st_current->maxoffset = offset;
		offset=0;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"input_plist"))
		f->ip_plist=0; 	
	if(current->tag==0 && !strcmp(current->name.nt_name,"output_plist"))
		f->op_plist=0; 													
	if(current->tag==0 && !strcmp(current->name.nt_name,"declareStmt"))
		f->dec=0;														
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDeclarations"))
		f->mod_dec=0;	
}


void print_activation_record(st_node* current)
{
	printf("%-25s %-20s\n", "Function Name", "Activation Record Size");
	for(int i=0; i<48; i++)
		printf("_");
	printf("\n\n");
	current = current->child;
	
	if(current->level == 0)
		current = current->child;
//currrent has main scope + oplist waala table
	while(current != NULL)
	{
		int subtract = 0;
		symbol* temp = current->output_head;
		while(temp != NULL)
		{
			subtract += temp->size;
			temp = temp->next_input_output;	
		}
		temp = current->parent->input_head;
		while(temp != NULL)
		{
			subtract += temp->size;
			temp = temp->next_input_output;	
		}	
		//current has main scope + oplist	
		printf("%-25s %-20d\n\n", current->func_name , current->maxoffset-subtract);
		if(current->parent->level != -1 /*current->parent->input_head != NULL*/)
		{
			current = current->parent;
			current = current->next;
			if(current != NULL && current->level == 0)
				current = current->child;
		}
		else
		{
			current = current->next;
			if(current != NULL && current->level == 0)
				current = current->child;	
		}
	}	
	return;
}


st_node* create_st(ast_tree* p)
{
	st_node* st_current=malloc(sizeof(st_node));
	st_current->head=malloc(sizeof(st_hashtable));
	st_current->func_name = malloc(sizeof(char)* 10);
	strcpy(st_current->func_name , "root");
	st_current->scope=0;
	st_current->parent=NULL;
	st_current->next=NULL;
	st_current->child=NULL;
	st_current->end_scope=-1;
	st_current->level=-1;
	st_current->output_head = NULL;
	st_current->input_head = NULL;
	flag* f=malloc(sizeof(flag));
	f->dec=0;
	f->ip_plist=0;
	f->op_plist=0;
	f->mod=0;
	f->mod_dec=0;
	scope=0;
	offset=0;
	for (int i=0;i<SIZEOF_ST_HASH_TABLE;i++)
		st_current->head->arr[i]=NULL;
	create_sym_tree(p->root,st_current,f);
	scope = 0;
	f->dec=0;
	f->ip_plist=0;
	f->op_plist=0;
	f->mod=0;
	f->mod_dec=0;
	track_func_scope = 0;
	Semantic_Check(p->root,st_current,f);
	Type_Checking(p);
	return st_current;
}


void * print_st(st_node* current)
{	
	symbol * temp=NULL;
	bool check = 0 ;
	if(!strcmp(current->func_name, "root"))
		check = 1;
	
	while(current!=NULL)
	{	for(int i=0;i<SIZEOF_ST_HASH_TABLE;i++)
		{	
			char * func_name= current->func_name;
			int scope=current->level;
			if(current->head->arr[i]!=NULL)
				temp =current->head->arr[i];
			while(temp!=NULL)
			{	
				int print_level = temp->output==1?current->level-1:current->level;
				if(temp->forvar==0)
				{
					if(temp->func==1)
					{
						if(check == 0)
							printf("%-5d %-25s %-25s %-14d %-14d %-8s %-10s %-16s %-20s %-13s %-10s %-10s\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, "----", "----", "----", "----", "----", "----", "----");
						else
						{
							st_node* t = current->child;
							while(t!=NULL && strcmp(t->func_name, temp->var_name))
								t = t->next;

							if(t != NULL)
								printf("%-5d %-25s %-25s %-14d %-14d %-8s %-10s %-16s %-20s %-13s %-10s %-10s\n",++symind, temp->var_name, func_name, temp->start_scope, t->end_scope, "----", "----", "----", "----", "----", "----", "----");
						}
					}
					else if(temp->func==0 && temp->arr==0)
						printf("%-5d %-25s %-25s %-14d %-14d %-8d %-10s %-16s %-20s %-13s %-10d %-10d\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, temp->size, "NO", "----", "----", temp->type, temp->offset-temp->size, print_level);

					else if(temp->arr==1 && temp->start_arr_dec==1 && temp->end_arr_dec==1)
						printf("%-5d %-25s %-25s %-14d %-14d %-8d %-10s %-16s [ %-4d, %-4d]\t  %-13s %-10d %-10d\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, temp->size, "YES", "STATIC", temp->start_arr, temp->end_arr, temp->type, temp->offset-temp->size, print_level);

					else if(temp->arr==1 && temp->start_arr_dec==0 && temp->end_arr_dec==0)
						printf("%-5d %-25s %-25s %-14d %-14d %-8d %-10s %-16s [ %-4s, %-4s]\t  %-13s %-10d %-10d\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, temp->size, "YES", "DYNAMIC", temp->arr_start,temp->arr_end, temp->type, temp->offset-temp->size, print_level);	

					else if(temp->arr==1 && temp->start_arr_dec==0 && temp->end_arr_dec==1)
						printf("%-5d %-25s %-25s %-14d %-14d %-8d %-10s %-16s [ %-4s, %-4d]\t  %-13s %-10d %-10d\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, temp->size, "YES", "DYNAMIC", temp->arr_start, temp->end_arr, temp->type, temp->offset-temp->size, print_level);	

					else
						printf("%-5d %-25s %-25s %-14d %-14d %-8d %-10s %-16s [ %-4d, %-4s]\t  %-13s %-10d %-10d\n",++symind, temp->var_name, func_name, temp->start_scope, current->end_scope, temp->size, "YES", "DYNAMIC", temp->start_arr,temp->arr_end, temp->type, temp->offset-temp->size, print_level);	
				}
				temp=temp->next;
			}
		}	
		if(current->child!=NULL)
			print_st(current->child);
			
		if(current->next!=NULL)
			current=current->next;
		else
			break;
	}
}



void* printSymbolTable(st_node* root)
{
	printf("\n%-5s %-25s %-25s %-14s %-14s %-8s %-10s %-16s %-20s %-13s %-10s %-10s\n","S.NO","Variable Name","Module Name","Start Scope","End Scope","Size","IsArray", "Static/Dynamic", "Range Variables", "Data Type","Offset", "Scope Level");
	for(int i=0; i<182; i++)
		printf("_");
	printf("\n");
	print_st(root);	
	printf("\n\n");
}


void * print_arr(st_node* current)
{	
	symbol * temp=NULL;
	bool check = 0 ;
	if(!strcmp(current->func_name, "root"))
		check = 1;
	
	while(current!=NULL)
	{	for(int i=0;i<SIZEOF_ST_HASH_TABLE;i++)
		{	
			char * func_name= current->func_name;
			int scope=current->level;
			if(current->head->arr[i]!=NULL)
				temp =current->head->arr[i];
			while(temp!=NULL && check == 0)
			{	
				int print_level = temp->output==1?current->level-1:current->level;
				if(temp->forvar==0)
				{
					if(temp->arr==1 && temp->start_arr_dec==1 && temp->end_arr_dec==1)
						printf("%-5d %-25s %-14d %-14d %-17s %-10d %-13s    [ %-4d, %-4d]     %-15s\n",++symind, func_name, temp->start_scope, current->end_scope, temp->var_name, temp->size,"STATIC", temp->start_arr, temp->end_arr, temp->type);

					else if(temp->arr==1 && temp->start_arr_dec==0 && temp->end_arr_dec==0)
						printf("%-5d %-25s %-14d %-14d %-17s %-10d %-13s    [ %-4s, %-4s]     %-15s\n",++symind, func_name, temp->start_scope, current->end_scope, temp->var_name, temp->size, "DYNAMIC", temp->arr_start,temp->arr_end, temp->type);	

					else if(temp->arr==1 && temp->start_arr_dec==0 && temp->end_arr_dec==1)
						printf("%-5d %-25s %-14d %-14d %-17s %-10d %-13s    [ %-4s, %-4d]     %-15s\n",++symind, func_name, temp->start_scope, current->end_scope, temp->var_name, temp->size, "DYNAMIC", temp->arr_start, temp->end_arr, temp->type);	

					else if(temp->arr==1 && temp->start_arr_dec==1 && temp->end_arr_dec==0)
						printf("%-5d %-25s %-14d %-14d %-17s %-10d %-13s    [ %-4d, %-4s]     %-15s\n",++symind, func_name, temp->start_scope, current->end_scope, temp->var_name, temp->size, "DYNAMIC", temp->start_arr,temp->arr_end, temp->type);	
				}
				temp=temp->next;
			}
		}	
		if(current->child!=NULL)
			print_arr(current->child);
			
		if(current->next!=NULL)
			current=current->next;
		else
			break;
	}
}



void* printArray(st_node* root)
{
	printf("\n\n%-5s %-25s %-14s %-14s %-17s %-10s %-13s   %-16s  %-15s\n","S.NO","Module Name","Start Scope","End Scope","Variable Name","Size", "Static/Dynamic", "Range Variables", "Element Type");
	for(int i=0; i<138; i++)
		printf("_");
	printf("\n");
	print_arr(root);
	symind = 0;
	printf("\n\n");		
}


//Compile time check for equality of two array symbols
int arr_check(symbol* s1 , symbol* s2)
{
	if(!s1->start_arr_dec && !s2->start_arr_dec && !s1->end_arr_dec && !s2->end_arr_dec)				//all ids no checking required
		return 0;

	else if(!s1->start_arr_dec && !s2->start_arr_dec && !s1->end_arr_dec && s2->end_arr_dec)			//error one is id other is num
		return -1;

	else if(!s1->start_arr_dec && !s2->start_arr_dec && s1->end_arr_dec && !s2->end_arr_dec)			//error one is id other is num
		return -2;

	else if(!s1->start_arr_dec && !s2->start_arr_dec && s1->end_arr_dec && s2->end_arr_dec)				//need to check equality of end
	{
		if(s1->end_arr == s2->end_arr) 
			return 3;
		else
			return -3;
	}
	else if(!s1->start_arr_dec && s2->start_arr_dec && !s1->end_arr_dec && !s2->end_arr_dec)			//error one is id other is num
		return -4;

	else if(!s1->start_arr_dec && s2->start_arr_dec && !s1->end_arr_dec && s2->end_arr_dec)				//error one is id other is num
		return -5;

	else if(!s1->start_arr_dec && s2->start_arr_dec && s1->end_arr_dec && !s2->end_arr_dec)				//error one is id other is num
		return -6;

	else if(!s1->start_arr_dec && s2->start_arr_dec && s1->end_arr_dec && s2->end_arr_dec)				//error one is id other is num
	{
		if(s1->end_arr == s2->end_arr)
			return 7;
		else
			return -7;
	}

	else if(s1->start_arr_dec && !s2->start_arr_dec && !s1->end_arr_dec && !s2->end_arr_dec)			//error one is id other is num
		return -8;

	else if(s1->start_arr_dec && !s2->start_arr_dec && !s1->end_arr_dec && s2->end_arr_dec)				//error one is id other is num
		return -9;

	else if(s1->start_arr_dec && !s2->start_arr_dec && s1->end_arr_dec && !s2->end_arr_dec)				//error one is id other is num
		return -10;

	else if(s1->start_arr_dec && !s2->start_arr_dec && s1->end_arr_dec && s2->end_arr_dec)				//error one is id other is num
	{	
		if(s1->end_arr == s2->end_arr)
			return 11;
		else 
			return -11;
	}
	else if(s1->start_arr_dec && s2->start_arr_dec && !s1->end_arr_dec && !s2->end_arr_dec)				//need to check equality of start
	{	
		if(s1->start_arr == s2->start_arr)
			return 12;
		else
			return -12;
	}
	else if(s1->start_arr_dec && s2->start_arr_dec && !s1->end_arr_dec && s2->end_arr_dec)				//error one is id other is num
	{	
		if(s1->start_arr == s2->start_arr)
			return 13;
		else
			return -13;
	}
	else if(s1->start_arr_dec && s2->start_arr_dec && s1->end_arr_dec && !s2->end_arr_dec)				//error one is id other is num
	{
		if(s1->start_arr == s2->start_arr)
			return 14;
		else
			return -14;
	}				
		

	else if(s1->start_arr_dec && s2->start_arr_dec && s1->end_arr_dec && s2->end_arr_dec)				//need to check both start and end
	{
		
		if(s1->start_arr == s2->start_arr && s1->end_arr && s2->end_arr)
			return 15;
		else 
			return -15;
	}
}



int for_lookahead(ast_node* current , char* forid)
{
	if((current->tag==0 && (!strcmp(current->name.nt_name,"assignmentStmt"))) || (current->tag == 1 && (!strcmp(current->name.l->TOKEN,"GET_VALUE"))))
	{
		char* search = current->tag==0?current->child->name.l->lexeme:current->next->name.l->lexeme;  
		int line = current->tag==0?current->child->name.l->Line_No:current->next->name.l->Line_No;
		if(!strcmp(search , forid))
		{
			return line;
		}
	}

	ast_node* child= current->child;
	while(child!=NULL)
	{
		int line = for_lookahead(child, forid);
		if(line != -1)
		{
			return line;
		}						
		child = child->next;
	}
	return -1;
}


void Semantic_Check(ast_node* p,st_node* st_current, flag* f)
{
	if(p==NULL)
		return ;
	ast_node* current=p;
	bool new_scope=0;
	
	//if current ast node is "module" non terminal  
	if(current->tag==0 && !strcmp(current->name.nt_name,"module"))
	{
		f->mod=1;
		scope++;
		st_current = st_current->child;
		while(scope != st_current->scope)
			st_current = st_current->next;
		new_scope=1;
	}
	//if current non terminal is "program" and it's child is a terminal for <<<driver program>>>
	if(current->tag==0 && !strcmp(current->name.nt_name,"program") && current->child->tag==1);
	//if current non terminal is module declaration
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDeclarations"))
		f->mod_dec=1;
	//if current is non terminal moduleDef of driver program as f->mod = 0
	if(current->tag==0 && (!strcmp(current->name.nt_name,"moduleDef")  && f->mod==0))
	{	
		scope++;
		track_func_scope = scope;
		st_current = st_current->child;
		while(scope != st_current->scope)
			st_current = st_current->next;
		new_scope=1;
	}
	//if current non terminal is conditional or iterative statement 
	if(!strcmp(current->name.nt_name,"conditionalStmt") || !strcmp(current->name.nt_name,"iterativeStmt")  )
	{
		scope++;		
		st_current = st_current->child;	
		while(scope != st_current->scope)
			st_current = st_current->next;
		new_scope=1;
	}

	//if current non terminal is moduleDef for any module except driver
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDef")&&  f->mod==1)
	{
		f->mod=0;
		if(st_current->parent->parent == NULL)
		{
			scope++;
			st_current = st_current->child;
		}
	}
	//if current non terminal is input_plist
	if(current->tag==0 && !strcmp(current->name.nt_name,"input_plist"))
		f->ip_plist=1;

	//if current non terminal is output_plist
	if(current->tag==0 && !strcmp(current->name.nt_name,"output_plist"))
	{
		scope++;		
		st_current=st_current->child;		
		f->op_plist=1;
		
	}
	//if current non terminal is declareStmt
	if(current->tag==0 && !strcmp(current->name.nt_name,"declareStmt"))
		f->dec=1;		
	
	//if current terminal is ID for module name
	if(f->mod_dec==1 && (current->tag==1) && !strcmp(current->name.l->TOKEN,"ID"));
		//insert_into_hashtable(temp,st_current);

	//if current terminal is ID and "DECLARE" has already come 
	if(current->tag==1 && f->dec==1 && !strcmp(current->name.l->TOKEN,"ID"));

	//the next token is int,real,bool meaning it is not an array
	if(current->tag==1 && f->dec==1 && (!strcmp(current->name.l->TOKEN,"INTEGER")||!strcmp(current->name.l->TOKEN,"REAL")||!strcmp(current->name.l->TOKEN,"BOOLEAN")))
	{
		if(current->tag==1)
			f->dec=0;
	}

	//next token is datatype meaing it is an array
	if(current->tag==0 && f->dec==1 && f->ip_plist==0 && f->op_plist==0 && !strcmp(current->name.nt_name,"dataType"))
		f->dec=0;

	//if current non terminal is iplist 
	if(current->tag==0 && f->ip_plist==1 && f->dec==0 && (!strcmp(current->name.nt_name,"iplist")||!strcmp(current->name.nt_name,"input_plist")));

	//if current non terminal is oplist
	if(current->tag==0 && f->op_plist==1 && f->dec==0 && (!strcmp(current->name.nt_name,"oplist")||!strcmp(current->name.nt_name,"output_plist")));

	//Type Check for function call 
	if(current->tag==0 && (!strcmp(current->name.nt_name,"moduleReuseStmt")))
	{			
		//No return but trying to be caught in variable
		st_node* t = NULL;
		if(current->childsize == 3)
			t = lookup_function_main_scope_table(current->child->next->name.l->lexeme , st_current, 0);
		else if(current->childsize == 2)
			t = lookup_function_main_scope_table(current->child->name.l->lexeme , st_current, 0);

		int line = current->childsize==3?current->child->next->name.l->Line_No:current->child->name.l->Line_No;

		if(t == NULL)
		{
			if(current->childsize==3)
			{
				printf(RED"ERROR: "RESET"Undefined reference to the function %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Function %s may not be defined\n\n",current->child->next->name.l->lexeme, current->child->next->name.l->Line_No, current->child->next->name.l->lexeme);
				SemErr++;
			}
			else
			{
				printf(RED"ERROR: "RESET"Undefined reference to the function %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Function %s may not be defined\n\n",current->child->name.l->lexeme, current->child->name.l->Line_No, current->child->name.l->lexeme);
				SemErr++;
			}
		}		
		else if(strcmp(t->func_name , st_current->func_name))
		{
			if(current->childsize != 2 && t->output_head == NULL)
			{
				printf(RED"ERROR: "RESET"Return Value caught in a variable for a void function "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Function %s does not return any value. Void value ignored as it ought to be\n\n",current->child->next->name.l->Line_No,current->child->next->name.l->lexeme);
				SemErr++;
			}

			if(current->childsize == 3)
			{
				symbol* output = lookup_function_main_scope_table(current->child->next->name.l->lexeme , st_current, 0)->output_head;
				ast_node* idlist = current->child;
				if(idlist->child->tag == 0)
					idlist = idlist->child->child;
				else if(idlist->child->tag == 1)
					idlist = idlist->child;
				
				while(1)
				{
					if((idlist == NULL || ((idlist->tag !=1) && !strcmp(idlist->name.l->lexeme , ":="))) || output == NULL)
					{
						printf(RED"ERROR: "RESET"Unequal number of parametes captured from the Module %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Number output parameters of module %s don't match number of actual parameters\n\n", current->child->next->name.l->lexeme, current->child->next->name.l->Line_No , current->child->next->name.l->lexeme);
						SemErr++;
						break;
					}
					else if(idlist->tag == 1 && idlist->symbol == NULL)
					{	
						printf(RED"ERROR: "RESET"Module Returning into Undeclared variable %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Each Undeclared Variable/Module is reported for each Scope it appears in\n\n" ,idlist->name.l->lexeme, idlist->name.l->Line_No);
						SemErr++;
					}

					else if(idlist->tag == 1 && !strcmp(idlist->symbol->type,  output->type) && (idlist->symbol->arr == output->arr))
					{
						if(idlist->symbol->arr == 1 && output->arr == 1)
						{
							printf(RED"ERROR: "RESET"Array %s cannot be returned by function %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array/Pointer Type Variables can't be returned\n\n", output->var_name, current->child->next->name.l->lexeme, current->child->next->name.l->Line_No );								
							SemErr++;
						}
					}
					else
					{
						printf(MAGENTA"WARNING: "RESET"Conflicting types for catching parameter %s and function parameter %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Such incompatible assignment may result in lossy conversion\n\n" , idlist->symbol->var_name, output->var_name, current->child->next->name.l->Line_No);
						SemErr++;
					}
					idlist = idlist->next;
					output = output->next_input_output;

					if(idlist==NULL && output == NULL)
						break;

					if(idlist != NULL && idlist->tag == 1 && !strcmp(idlist->name.l->lexeme , ":=") && output == NULL)
						break;
					
					if(idlist != NULL && idlist->tag == 0 && !strcmp(idlist->name.nt_name, "idLists"))
						idlist = idlist->child;
				}
			}
			
			symbol* formal_param;
			ast_node* actual_param;
			if(current->childsize == 3)
			{
				formal_param = lookup_function_main_scope_table(current->child->next->name.l->lexeme , st_current, 1)->input_head;
				actual_param = current->child->next->next;	
			}
			else if(current->childsize == 2)
			{
				formal_param = lookup_function_main_scope_table(current->child->name.l->lexeme , st_current, 1)->input_head;
				actual_param = current->child->next;
			}
			if(actual_param->tag==0)
				actual_param=actual_param->child;
			while(1)
			{
				if(actual_param == NULL || formal_param == NULL)
				{
					if(current->childsize == 2)
					{
						printf(RED"ERROR: "RESET"Unequal number of parametes captured from the Module %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Number output parameters of module %s don't match number of actual parameters\n\n", current->child->name.l->lexeme, current->child->name.l->Line_No, current->child->name.l->lexeme);
						SemErr++;
						break;
					}
					else if(current->childsize == 3)
					{
						printf(RED"ERROR: "RESET"Unequal number of parametes captured from the Module %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Number output parameters of module %s don't match number of actual parameters\n\n", current->child->next->name.l->lexeme, current->child->next->name.l->Line_No, current->child->next->name.l->lexeme);
						SemErr++;
						break;
					}
					break;
				}
				else if(actual_param->tag == 1 && actual_param->symbol == NULL)
				{
					printf(RED"ERROR: "RESET"%s undeclared "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Each Undeclared Variable/Module is reported for each Scope it appears in\n\n" ,actual_param->name.l->lexeme, actual_param->name.l->Line_No);
					SemErr++;
				}
				else if(actual_param->tag == 1 && !strcmp(actual_param->symbol->type,  formal_param->type) && actual_param->symbol->arr == formal_param->arr)
				{
					if(actual_param->symbol->arr == 1 && formal_param->arr == 1)
					{
						int see = arr_check(actual_param->symbol , formal_param);
						if(see >= 0);

						else if(see == -3 || see == -7 || see == -11)
						{
							//end_arr mismatch
							printf(RED"ERROR: "RESET"Array Index Mismatch "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"%d and %d for %s do not match\n\n", line , actual_param->symbol->end_arr, formal_param->end_arr, actual_param->symbol->var_name);
							SemErr++;
						}
						else if(see == -12 || see == -13 || see == -14)
						{
							//start arr mismatch
							printf(RED"ERROR: "RESET"Array Index Mismatch "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"%d and %d for %s do not match\n\n", line , actual_param->symbol->start_arr, formal_param->start_arr, actual_param->symbol->var_name);
							SemErr++;
						}
						else if(see == -15)
						{
							//start and/or end mismatch
							printf(RED"ERROR: "RESET"Array Index Mismatch "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Start and End indices for %s do not match\n\n", line , actual_param->symbol->var_name);
							SemErr++;
						}
					}
				}
				else
				{
					if(current->childsize == 2)
					{
						printf(MAGENTA"WARNING: "RESET"Conflicting types for catching parameter %s and function parameter %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Such incompatible assignment may result in lossy conversion\n\n" , actual_param->symbol->var_name, formal_param->var_name, current->child->name.l->Line_No);
						SemErr++;
					}
					else if(current->childsize == 3)
					{
						printf(MAGENTA"WARNING: "RESET"Conflicting types for catching parameter %s and function parameter %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Such incompatible assignment may result in lossy conversion\n\n" , actual_param->symbol->var_name, formal_param->var_name, current->child->next->name.l->Line_No);		
						SemErr++;
					}
				}
				
				formal_param = formal_param->next_input_output;
				actual_param = actual_param->next;
				if(actual_param == NULL && formal_param == NULL)
					break;
				if(actual_param!=NULL && actual_param->tag == 0 && !strcmp(actual_param->name.nt_name, "idLists"))
						actual_param = actual_param->child;
			}
		}
		else
		{
			printf(RED"ERROR: "RESET"Recursion not allowed "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Function %s is trying to call itself\n\n", line , t->func_name);								
			SemErr++;
		}
	}

	if(current->tag==0 && (!strcmp(current->name.nt_name,"conditionalStmt")))
	{
		if(current->child->symbol != NULL)
		{
			if(current->child->tag == 1 && !strcmp(current->child->symbol->type, "REAL"))
			{
				printf(RED"ERROR: "RESET"Switch Quantity cannot have REAL Data Type "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Switch Variable can only be INTEGER/BOOLEAN\n\n" , current->child->name.l->Line_No);
				SemErr++;
			}
			if(current->child->tag == 1 && !strcmp(current->child->symbol->type, "BOOLEAN") && (current->childsize == 5))
			{
				printf(RED"ERROR: "RESET"Switch on a BOOLEAN Quantity cannot have a DEFAULT Case "LINE"in Line Number: %d"RESET"\n\n" , current->child->name.l->Line_No);
				SemErr++;
			}

			if(current->child->tag == 1 && !strcmp(current->child->symbol->type, "INTEGER") && (current->childsize != 5))
			{
				printf(RED"ERROR: "RESET"Switch on an INTEGER Quantity should have a DEFAULT Case "LINE"in Line Number: %d"RESET"\n\n" , current->child->name.l->Line_No);
				SemErr++;
			}
			
			if(current->child->next->next->tag == 1)
			{
				if(!strcmp(current->child->symbol->type, "INTEGER"))
				{
					if(!strcmp(current->child->next->next->name.l->TOKEN, "NUM"));

					else
					{
						printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "INTEGER", current->child->next->next->name.l->Line_No);
						SemErr++;
					}

				}
				else if(!strcmp(current->child->symbol->type, "BOOLEAN"))
				{
					if(!strcmp(current->child->next->next->name.l->TOKEN, "TRUE") || !strcmp(current->child->next->next->name.l->TOKEN, "FALSE"));

					else
					{
						printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "BOOLEAN", current->child->next->next->name.l->Line_No);
						SemErr++;
					}
				}
			}
			else
			{
				ast_node* cas = current->child->next->next;
				if(current->child->tag == 1 && !strcmp(current->child->symbol->type, "INTEGER"))
				{
					while(cas != NULL)
					{
						if(!strcmp(cas->child->name.l->TOKEN,"NUM"));

						else
						{
							printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "INTEGER", cas->child->name.l->Line_No);
							SemErr++;
						}
						
						if(cas->childsize == 3)
						{
							cas = cas->child->next->next;
							if(cas != NULL && cas->tag == 1)
							{
								if(!strcmp(cas->name.l->TOKEN, "NUM"));

								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "INTEGER", cas->name.l->Line_No);
									SemErr++;
								}
								cas = NULL;
							}
						}
						else if(cas->childsize == 2)
						{																								//either statements is null or casestmt is null
							if(cas->child->next->tag == 0 && !strcmp(cas->child->next->name.nt_name, "caseStmt"))
								cas = cas->child->next;

							else if(cas != NULL && cas->child->next->tag == 1)
							{
								if(!strcmp(cas->child->name.l->TOKEN, "NUM"));

								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "INTEGER", cas->child->name.l->Line_No);
									SemErr++;
								}

								if(!strcmp(cas->child->next->name.l->TOKEN, "NUM"));

								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "INTEGER", cas->child->next->name.l->Line_No);
									SemErr++;
								}
								cas = NULL;
							}
							else	
								cas = NULL;
						}
					}	
				}
			
				if(current->child->tag == 1 && !strcmp(current->child->symbol->type, "BOOLEAN"))
				{
					int count = 2;
					while(cas != NULL)
					{
						if(!strcmp(cas->child->name.l->TOKEN,"TRUE") || !strcmp(cas->child->name.l->TOKEN,"FALSE"));
						
						else
						{
							printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "BOOLEAN", cas->child->name.l->Line_No);
							SemErr++;
						}

						if(cas->childsize == 3)
						{
							cas = cas->child->next->next;
							if(cas != NULL && cas->tag == 1)
							{
								if(!strcmp(cas->name.l->TOKEN, "TRUE") || !strcmp(cas->name.l->TOKEN, "FALSE"));

								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "BOOLEAN", cas->name.l->Line_No);
									SemErr++;
								}
								cas = NULL;
							}
						}
						else if(cas->childsize == 2)
						{
							if(cas->child->next->tag == 0 && !strcmp(cas->child->next->name.nt_name, "caseStmt"))
								cas = cas->child->next;
				
							else if(cas != NULL && cas->child->next->tag == 1)
							{
								if(!strcmp(cas->child->name.l->TOKEN, "TRUE") || !strcmp(cas->child->name.l->TOKEN, "FALSE") )
									count--;
								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "BOOLEAN", cas->child->name.l->Line_No);
									SemErr++;
								}
								
								if(!strcmp(cas->child->next->name.l->TOKEN, "TRUE") || !strcmp(cas->child->next->name.l->TOKEN, "FALSE"))
									count--;
								else
								{
									printf(RED"ERROR: "RESET"Conflicting Types for SWITCH Variable %s and CASE Quantity "LINE"in Line Number: %d"RESET"\n\n" , "BOOLEAN", cas->child->next->name.l->Line_No);
									SemErr++;
								}
								cas = NULL;
								break;
							}
							else	
								cas = NULL;
						}
						count--;
					}
					if(count < 0)
					{
						printf(RED"ERROR: "RESET"More than two cases in Boolean Switch "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"SWITCH Quantity of type BOOLEAN can have only two cases\n\n" ,current->child->name.l->Line_No);
						SemErr++;
					}
				}
			}
		}
		else
		{
			printf(RED"ERROR: "RESET"Switch Variable %s undeclared "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Each Undeclared Variable is reported only once for each Scope it appears in\n\n",current->child->name.l->lexeme, current->child->name.l->Line_No);
			SemErr++;
		}
	}

	if(current->tag == 1 && !strcmp(current->name.l->TOKEN , "FOR"))
	{
		if(current->next != NULL && current->next->tag == 1 && current->next->symbol != NULL && strcmp(current->next->symbol->type, "INTEGER"))
		{
			printf(RED"ERROR: "RESET"Incompatible type for FOR variable %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"For loop variable can only be an INTEGER Type\n\n",current->next->name.l->lexeme, current->next->name.l->Line_No);
			SemErr++;
		}
		else if(current->next != NULL && current->next->tag == 1 && current->next->symbol != NULL &&  !strcmp(current->next->symbol->type, "INTEGER"))
		{
			char* forid = current->next->name.l->lexeme;
			if(current->next->next->next->next->tag == 0)
			{
				int line = for_lookahead(current->next->next->next->next, forid);
				if(line != -1)
				{
					printf(RED"ERROR: "RESET"FOR loop variable %s Assigned "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"FOR Loop iterator cannot be Assigned in the Loop it is in\n\n",current->next->name.l->lexeme, line);
					SemErr++;
				}
			}
		}
	}
	ast_node* child= p->child;

	while(child!=NULL)
	{
		Semantic_Check(child,st_current,f);						
		child = child->next;
	}

	
	if(current->tag==0 && !strcmp(current->name.nt_name,"input_plist"))
		f->ip_plist=0; 	
	if(current->tag==0 && !strcmp(current->name.nt_name,"output_plist"))
		f->op_plist=0; 																
	if(current->tag==0 && !strcmp(current->name.nt_name,"declareStmt"))
		f->dec=0;																	
	if(current->tag==0 && !strcmp(current->name.nt_name,"moduleDeclarations"))
		f->mod_dec=0;
	return;
}


bool is_same_type(type* ty1, type* ty2)
{
	if(!strcmp(ty1->node_type,ty2->node_type) && ty1->arr==ty2->arr && ty1->start_arr==ty2->start_arr && ty1->end_arr==ty2->end_arr)
		return 1;
	else 
		return 0;
}


void type_check(ast_node* p)
{
	if(p==NULL)
		return;
	ast_node* child= p->child;
	//traverse children
	while(child!=NULL)
	{
		type_check(child);						
		child = child->next;
	}
	ast_node* current=p;
	if(current->tag==1 && !strcmp(current->name.l->TOKEN,"ID"))
	{
		if(current->symbol == NULL)
			current->ty=NULL;
		else
		{	
			current->ty=malloc(sizeof(type));
			current->ty->arr=current->symbol->arr;
			current->ty->start_arr=current->symbol->start_arr;
			current->ty->end_arr=current->symbol->end_arr;
			current->ty->node_type=current->symbol->type;
			current->ty->Line_No=current->name.l->Line_No;
		}
	}
	if(current->tag==1 && !strcmp(current->name.l->TOKEN,"NUM"))
	{
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(8));
		strcpy(current->ty->node_type,"INTEGER");
		current->ty->Line_No=current->name.l->Line_No;
	}
	if(current->tag==1 && !strcmp(current->name.l->TOKEN,"RNUM"))
	{
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(5));
		strcpy(current->ty->node_type,"REAL");
		current->ty->Line_No=current->name.l->Line_No;
	}
	if(current->tag==1 && (!strcmp(current->name.l->TOKEN,"TRUE") || !strcmp(current->name.l->TOKEN,"FALSE")))
	{		
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(8));
		strcpy(current->ty->node_type,"BOOLEAN");
		current->ty->Line_No=current->name.l->Line_No;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"assignmentStmt"))
	{
		if(current->child->next->child->tag==1 && !strcmp(current->child->next->child->name.l->TOKEN, "ASSIGNOP"))
		{
			if(current->child->ty!=NULL && current->child->next->child->next->ty!=NULL)
			{		
				if(strcmp(current->child->ty->node_type,current->child->next->child->next->ty->node_type))
				{
					printf(RED"ERROR: "RESET"Type Mismatch between operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Left Operand of type %s whereas Right Operand(s) of type %s may result in lossy conversion\n\n" ,current->child->ty->Line_No,current->child->ty->node_type,current->child->next->child->next->ty->node_type);
					SemErr++;
				}

				else if(current->child->ty->arr!=current->child->next->child->next->ty->arr)		
				{
					if(current->child->ty->arr==1)
					{
						printf(RED"ERROR: "RESET"Type Mismatch between operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Can't assign Array pointer type to primitive data type\n\n",current->child->ty->Line_No);
						SemErr++;
					}
						
					else
					{
						printf(RED"ERROR: "RESET"Type Mismatch between operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Can't assign primitive data type to an Array pointer type\n\n",current->child->ty->Line_No);
						SemErr++;
					}
										
				}		
				else if(current->child->ty->arr==1 && current->child->next->child->next->ty->arr==1 && current->child->tag==1 && current->child->next->child->next->tag==1 && ((current->child->symbol->start_arr_dec==1 && current->child->next->child->next->symbol->start_arr_dec==1 && current->child->ty->start_arr!=current->child->next->child->next->ty->start_arr) || (current->child->symbol->end_arr_dec==1 && current->child->next->child->next->symbol->end_arr_dec==1 &&current->child->ty->end_arr!=current->child->next->child->next->ty->end_arr)))
				{
					printf(MAGENTA"WARNING: "RESET"Bound Mismatch between Array operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Left Operand and Right Operand of Array type have different bounds\n\n", current->child->ty->Line_No);					
					SemErr++;
				}
			}	
		}
		if(current->child->next->child->tag==1 &&  !strcmp(current->child->next->child->name.l->TOKEN, "NUM"))
		{
			int index=current->child->next->child->name.l->Value.val;
			if(current->child->ty!=NULL)
			{
				if(current->child->ty->arr==0)
				{
					printf(RED"ERROR: "RESET"Variable %s is neither array nor pointer nor vector type "LINE"in Line Number: %d"RESET"\n\n",current->child->name.l->lexeme,current->child->ty->Line_No);
					SemErr++;
				}
				else if((current->child->tag==1 && current->child->symbol!=NULL && current->child->symbol->start_arr_dec==1 && index<current->child->ty->start_arr) || (current->child->tag==1 && current->child->symbol!=NULL && current->child->symbol->end_arr_dec==1 && index>current->child->ty->end_arr))
				{
					printf(RED"ERROR: "RESET"Array index out of bounds for Array variable %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Accessing memory out of access may cause Segmentation Fault\n\n", current->child->name.l->lexeme, current->child->ty->Line_No);
					SemErr++;
				}
				else if(current->child->next->child->next->next->ty!=NULL && strcmp(current->child->ty->node_type,current->child->next->child->next->next->ty->node_type))
				{
					printf(RED"ERROR: "RESET"Type Mismatch between operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Left Operand of type %s whereas Right Operand(s) of type %s may result in lossy conversion\n\n", current->child->ty->Line_No, current->child->ty->node_type, current->child->next->child->next->next->ty->node_type);
					SemErr++;
				}
				else if(current->child->next->child->next->next->ty!=NULL && current->child->next->child->next->next->ty->arr==1)
				{
					printf(RED"ERROR: "RESET"Type Mismatch between operands of assignment statement "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Left Operand of type %s whereas Right Operand(s) of Array or Pointer type may result in lossy conversion\n\n", current->child->ty->Line_No, current->child->ty->node_type);
					SemErr++;
				}
			}
		}
		else if(current->child->next->child->tag==1 &&  !strcmp(current->child->next->child->name.l->TOKEN, "ID") )
		{			
			if( current->child->ty!=NULL && current->child->ty->arr==0)
			{
				printf(RED"ERROR: "RESET"Variable %s is neither array nor pointer nor vector type "LINE"in Line Number: %d"RESET"\n\n",current->child->name.l->lexeme, current->child->ty->Line_No);
				SemErr++;
			}
			if(current->child->next->child->ty!=NULL && strcmp(current->child->next->child->ty->node_type,"INTEGER"))
			{
				printf(RED"ERROR: "RESET"Array Subscript %s is not of an INTEGER Type "LINE"in Line Number: %d"RESET"\n\n", current->child->next->child->name.l->lexeme, current->child->next->child->ty->Line_No);
				SemErr++;
			}
		}
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"U"))
	{
		if(current->child->next->ty!=NULL)		
			current->ty=current->child->next->ty;
		else
			current->ty=NULL;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"arithmeticOrBooleanExpression"))
	{
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(8));
		strcpy(current->ty->node_type,"BOOLEAN");
		if(current->child->next->child->tag==1)
			current->ty->Line_No=current->child->next->child->name.l->Line_No;
		if(current->child->ty!=NULL)
		{
			current->ty->Line_No=current->child->ty->Line_No;
			if(current->child->ty->arr==1)
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Logical Operators not allowed on array variables\n\n",current->child->ty->Line_No);
				SemErr++;
			}
			else if(strcmp(current->child->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Non-Boolean expressions "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Logical Operators not allowed on %s expressions\n\n",current->child->ty->Line_No,current->child->ty->node_type);
				SemErr++;
			}
		}
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"followingBool"))
	{
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(8));
		strcpy(current->ty->node_type,"BOOLEAN");
		if(current->child->tag==1)
			current->ty->Line_No =current->child->name.l->Line_No;
		if(current->child->next->ty!=NULL)
		{
			current->ty->Line_No=current->child->next->ty->Line_No;
			if(current->child->next->ty->arr==1 )
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Logical Operators not allowed on Array variables\n\n",current->child->next->ty->Line_No);
				SemErr++;
			}
			else if(strcmp(current->child->next->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Non-Boolean expressions "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Logical Operators not allowed on %s expressions\n\n",current->child->next->ty->Line_No,current->child->next->ty->node_type);
				SemErr++;
			}
		}
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"boolTerm"))
	{
		current->ty=malloc(sizeof(type));
		current->ty->arr=0;
		current->ty->start_arr=0;
		current->ty->end_arr=0;
		current->ty->node_type=malloc(sizeof(8));
		strcpy(current->ty->node_type,"BOOLEAN");
		if(current->child->next->child->tag==1)
			current->ty->Line_No=current->child->next->child->name.l->Line_No;
		if(current->child->ty!=NULL)
		{
			current->ty->Line_No=current->child->ty->Line_No;
			if(current->child->ty->arr==1 )
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on Array variables\n\n",current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on BOOLEAN variables\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on MODULE Names\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(current->child->next->ty!=NULL && strcmp(current->child->ty->node_type,current->child->next->ty->node_type))
			{
				printf(RED"ERROR: "RESET"Incompatible Operation on Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relational Operation not allowed between %s and %s variables\n\n",current->child->ty->Line_No,current->child->ty->node_type,current->child->next->ty->node_type);
				SemErr++;
			}
		}
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"boolean"))
	{
		if(current->child->next->ty!=NULL)
		{
			if(current->child->next->ty->arr==1)
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on Array variables\n\n",current->child->next->ty->Line_No);						
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on BOOLEAN variables\n\n", current->child->next->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Relaional Operators not allowed on MODULE Names\n\n", current->child->next->ty->Line_No);
				SemErr++;
			}
		}
		current->ty=current->child->next->ty;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"arithmeticExpr") )
	{
		
		if(current->child->ty!=NULL)
		{
			if(current->child->ty->arr==1 )
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on Array variables\n\n",current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on BOOLEAN variables\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on MODULE Names\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(current->child->next->ty!=NULL && strcmp(current->child->ty->node_type,current->child->next->ty->node_type))
			{
				printf(RED"ERROR: "RESET"Incompatible Operation on Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operation not allowed between %s and %s variables\n\n",current->child->ty->Line_No,current->child->ty->node_type,current->child->next->ty->node_type);			
				SemErr++;
			}
		}		
		current->ty=current->child->ty;
		
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"followingArithExp") )
	{
		if(current->child->next->ty!=NULL)
		{		
			if(current->child->next->ty->arr==1 )
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on Array variables\n\n",current->child->next->ty->Line_No);	
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on BOOLEAN variables\n\n", current->child->next->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on MODULE Names\n\n", current->child->next->ty->Line_No);
				SemErr++;
			}
			else if(current->child->next->next!=NULL && current->child->next->next->ty!=NULL)
			{	
				if(strcmp(current->child->next->ty->node_type,current->child->next->next->ty->node_type))
				{
					printf(RED"ERROR: "RESET"Incompatible Operation on Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operation not allowed between %s and %s variables\n\n", current->child->next->ty->Line_No, current->child->next->ty->node_type,current->child->next->next->ty->node_type);
					SemErr++;
				}
			}
		}
		current->ty=current->child->next->ty;
		
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"term"))
	{
		if(current->child->ty!=NULL)
		{		
			if(current->child->ty->arr==1 )
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on Array variables\n\n",current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on BOOLEAN variables\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on MODULE Names\n\n", current->child->ty->Line_No);
				SemErr++;
			}
			else if(current->child->next->ty!=NULL && strcmp(current->child->ty->node_type,current->child->next->ty->node_type))
			{
				printf(RED"ERROR: "RESET"Incompatible Operation on Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operation not allowed between %s and %s variables\n\n", current->child->ty->Line_No ,current->child->ty->node_type,current->child->next->ty->node_type);
				SemErr++;
			}
		}
		current->ty=current->child->ty;
		
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"followingTerm"))
	{
		if(current->child->next->ty!=NULL)
		{		
			if(current->child->next->ty->arr==1)
			{
				printf(RED"ERROR: "RESET"Invalid Operation on Array Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on Array variables\n\n",current->child->next->ty->Line_No);
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on BOOLEAN Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on BOOLEAN variables\n\n", current->child->next->ty->Line_No);	
				SemErr++;
			}
			else if(!strcmp(current->child->next->ty->node_type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Operation on MODULE Name "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operators not allowed on MODULE Names\n\n", current->child->next->ty->Line_No);	
				SemErr++;
			}
			else if(current->child->next->next!=NULL && current->child->next->next->ty!=NULL)
			{
				if(strcmp(current->child->next->ty->node_type,current->child->next->next->ty->node_type))
				{
					printf(RED"ERROR: "RESET"Incompatible Operation on Variables "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Arithmetic Operation not allowed between %s and %s variables\n\n", current->child->next->ty->Line_No, current->child->next->ty->node_type, current->child->next->next->ty->node_type);
					SemErr++;
				}
			}
		}
		current->ty=current->child->next->ty;
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"var"))
	{
		
		if(current->child->ty!=NULL)
		{
			current->ty=malloc(sizeof(type));
			current->ty->arr=0;
			current->ty->start_arr=0;
			current->ty->end_arr=0;			
			current->ty->node_type=current->child->ty->node_type;
			current->ty->Line_No=current->child->ty->Line_No;
		}
		else 	
			current->ty=NULL;
		if(current->child->ty!=NULL && current->child->next->tag==1 && !strcmp(current->child->next->name.l->TOKEN,"NUM") )
		{
			int index=atoi(current->child->next->name.l->lexeme);
			if((current->child->tag==1 && current->child->symbol!=NULL && current->child->symbol->start_arr_dec==1 && index<current->child->ty->start_arr) || (current->child->tag==1 && current->child->symbol!=NULL && current->child->symbol->end_arr_dec==1 && index>current->child->ty->end_arr))
			{
				printf(RED"ERROR: "RESET"Array index out of bounds for Array variable %s "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Accessing memory out of access may cause Segmentation Fault\n\n",current->child->name.l->lexeme,current->child->ty->Line_No);
				SemErr++;
			}
		}
		else if(current->child->next->ty!=NULL && current->child->next->tag==1 && !strcmp(current->child->next->name.l->TOKEN,"ID") )
		{
			if(strcmp(current->child->next->ty->node_type, "INTEGER"))
			{
				printf(RED"ERROR: "RESET"Array Subscript %s is not of an INTEGER Type "LINE"in Line Number: %d"RESET"\n\n", current->child->next->name.l->lexeme, current->child->ty->Line_No);
				SemErr++;
			}
		}
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"iterativeStmt") && !strcmp(current->child->name.l->TOKEN,"WHILE"))
	{
		if(current->child->next->ty!=NULL)
		{
			if(strcmp(current->child->next->ty->node_type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Type Mismatch in While Condition "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Expression inside WHILE Condition should be a BOOLEAN expression\n\n", current->child->next->ty->Line_No);		
				SemErr++;
			}
		}						
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"dataType") )
	{
		if(current->child->child->tag==1 && !strcmp(current->child->child->name.l->TOKEN,"ID"))
		{
			if(current->child->child->symbol!= NULL && current->child->child->symbol->arr==1)
			{
				printf(RED"ERROR: "RESET"Invalid Start Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Array Type. No possible conversion from (int *) to int , may lead to Segmentation Fault\n\n", current->child->child->name.l->Line_No,current->child->child->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->symbol!= NULL && !strcmp(current->child->child->symbol->type,"REAL"))
			{
				printf(RED"ERROR: "RESET"Invalid Start Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Real Type. May lead to invalid Array bounds and Segmentation Fault\n\n", current->child->child->name.l->Line_No, current->child->child->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->symbol!= NULL && !strcmp(current->child->child->symbol->type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid Start Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Boolean Type. May lead to invalid Array bounds and Segmentation Fault\n\n",current->child->child->name.l->Line_No, current->child->child->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->symbol!= NULL && !strcmp(current->child->child->symbol->type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid Start Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be a Module name. May lead to invalid Array bounds and Segmentation Fault\n\n",current->child->child->name.l->Line_No, current->child->child->name.l->lexeme);
				SemErr++;
			}
		}
		
		if(current->child->child->next->tag==1 && !strcmp(current->child->child->next->name.l->TOKEN,"ID"))
		{
			if(current->child->child->next->symbol!= NULL && current->child->child->next->symbol->arr==1)
			{
				printf(RED"ERROR: "RESET"Invalid End Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Array Type. No possible conversion from (int *) to int, may lead to Segmentation Fault\n\n", current->child->child->next->name.l->Line_No,current->child->child->next->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->next->symbol!= NULL && !strcmp(current->child->child->next->symbol->type,"REAL"))
			{
				printf(RED"ERROR: "RESET"Invalid End Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Real Type. May lead to invalid Array bounds and Segmentation Fault\n\n", current->child->child->next->name.l->Line_No,current->child->child->next->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->next->symbol!= NULL && !strcmp(current->child->child->next->symbol->type,"BOOLEAN"))
			{
				printf(RED"ERROR: "RESET"Invalid End Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be Boolean Type. May lead to invalid Array bounds and Segmentation Fault\n\n", current->child->child->next->name.l->Line_No, current->child->child->next->name.l->lexeme);
				SemErr++;
			}
			else if(current->child->child->next->symbol!= NULL && !strcmp(current->child->child->next->symbol->type,"MODULE"))
			{
				printf(RED"ERROR: "RESET"Invalid End Bound on Array "LINE"in Line Number: %d"RESET"\n\t"CYAN"NOTE: "RESET"Array Index %s can't be a Module name. May lead to invalid Array bounds and Segmentation Fault\n\n", current->child->child->next->name.l->Line_No, current->child->child->next->name.l->lexeme);
				SemErr++;
			}		
		}			
	}
	if(current->tag==0 && !strcmp(current->name.nt_name,"ioStmt") )
	{
				
		if(current->child->next->ty!=NULL && current->child->next!=NULL && current->child->next->tag==1 && !strcmp(current->child->next->name.l->TOKEN,"ID") && !strcmp(current->child->next->symbol->type,"MODULE"))	
		{
			printf(RED"ERROR: "RESET"Invalid request for %s of ID of type module "LINE"in Line Number: %d"RESET"\n",current->child->name.l->TOKEN ,current->child->next->name.l->Line_No);
			SemErr++;		
		}
	}
}


void Type_Checking(ast_tree* p)
{
	ast_node* node = p->root;
	type_check(node);
}
