/*c
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "parser.h"
#include "lexer.h"


int last_occurence(char* s,grammar G)
{
	for(int i=G.size-1;i>=0;i--)
		if(strcmp(G.start[i].value,s)==0)
			return i;
}


int first_occurence(char* s,grammar G)
{
	for(int i=0;i<G.size;i++)
		if(strcmp(G.start[i].value,s)==0)
			return i;
}


int checkntort(char * ch, char ** strs,grammar G)
{
	if(strcmp(ch,"#")==0)
		return -1;	
	for(int i = 0; i < G.no_of_nt; i++)
	{
		if(!strcmp(strs[i], ch))
			return 0;
	}
	return 1;
}


void find_nt(grammar G)
{
	for(int i=0;i<G.size;i++)
	{
		RHS* head=G.start[i].head;
		while(head!=NULL)
		{
			head->check=checkntort(head->value,nonterminals,G);
			head=head->next;
		}	
	}
}


void populate_nt(grammar *G)
{
	if(G->size==1)
	{
		nonterminals=realloc(nonterminals,sizeof(char*)*(G->no_of_nt+1));
		nonterminals[G->no_of_nt]=malloc(sizeof(char)*(strlen(G->start[G->size-1].value)+1));
		strcpy(nonterminals[G->no_of_nt],G->start[G->size-1].value);
		G->no_of_nt++;		
	}	
	else if(strcmp(nonterminals[G->no_of_nt-1],G->start[G->size-1].value)!=0)
	{
		nonterminals=realloc(nonterminals,sizeof(char*)*(G->no_of_nt+1));
		nonterminals[G->no_of_nt]=malloc(sizeof(char)*(strlen(G->start[G->size-1].value)+1));
		strcpy(nonterminals[G->no_of_nt],G->start[G->size-1].value);
		G->no_of_nt++;		
	}
}


int calculate_hash(char* s,int factor)
{
	int sum=0;	
	for(int i=0;i<strlen(s);i++)
		sum += s[i];
	return sum%factor;
}


void fill_hash_nt_table()
{
	for(int i=0;i<gram.no_of_nt;i++)
	{
		int t=calculate_hash(nonterminals[i],factor_nt);
		hash_nt* start = hash_nt_ptr[t];
		if(start==NULL)
		{
			hash_nt_ptr[t]=malloc(sizeof(hash_nt));
			hash_nt_ptr[t]->value=nonterminals[i];
			hash_nt_ptr[t]->index=i;
			hash_nt_ptr[t]->next=NULL;
			continue;
		}		
		while(start->next!=NULL)
			start=start->next;
		
		start->next=malloc(sizeof(hash_nt));
		start->next->value=nonterminals[i];
		start->next->index=i;
		start->next->next=NULL;
		continue;
	}
}


void fill_hash_t_table()
{
	for(int i=0;i<no_of_terminals;i++)
	{
		int t=calculate_hash(terminals[i],factor_t);
		hash_t* start = hash_t_ptr[t];
		if(start==NULL)
		{
			hash_t_ptr[t]=malloc(sizeof(hash_t));
			hash_t_ptr[t]->value=terminals[i];
			hash_t_ptr[t]->index=i;
			hash_t_ptr[t]->next=NULL;
			continue;
		}		
		while(start->next!=NULL)
			start=start->next;	
	
		start->next=malloc(sizeof(hash_t));
		start->next->value=terminals[i];
		start->next->index=i;
		start->next->next=NULL;
		continue;
	}
}


bool already_present(char** arr, char* str,int size)
{
	for(int i=0;i<size;i++)
		if(strcmp(arr[i],str)==0)
			return 1;
	return 0;
}


int rank(char* nt)
{
	int t=calculate_hash(nt,factor_nt);
	hash_nt* start=hash_nt_ptr[t];
	while(start!=NULL )
	{
		if(strcmp(start->value,nt)==0)
			return start->index;
		start=start->next;	
	}
	return -1;
}


int terminal_rank(char* s)
{
	int t=calculate_hash(s,factor_t);
	hash_t* start=hash_t_ptr[t];
	while(start!=NULL)
	{
		if(strcmp(start->value,s)==0)
			return start->index;
		start=start->next;	
	}
	return -1;	

}

int fill_terminal(char* s,int size)
{
	if(already_present(terminals,s,size))
		return size;
	terminals=realloc(terminals,sizeof(char*)*(size+1));
	terminals[size]=malloc(strlen(s)+1);
	strcpy(terminals[size],s);
	size++;
	return size;
}

void find_follow(FirstAndFollow F, int i, RHS* start, LHS l,grammar G)
{
	if(start==NULL || start->check==-1)
	{
		int t=rank(l.value);		
		for(int k=0;k<F.follow[t].size_follow_list;k++)
		{
			if(already_present(F.follow[i].follow_list,F.follow[t].follow_list[k],F.follow[i].size_follow_list))
				continue;
			if(F.follow[i].size_follow_list==0)
				F.follow[i].follow_list=malloc(sizeof(char*));
			else
				F.follow[i].follow_list=realloc(F.follow[i].follow_list,sizeof(char*)*(F.follow[i].size_follow_list+1));	
			F.follow[i].follow_list[F.follow[i].size_follow_list]=malloc(strlen(F.follow[t].follow_list[k])+1);
			strcpy(F.follow[i].follow_list[F.follow[i].size_follow_list],F.follow[t].follow_list[k]);
			if(strcmp(F.follow[t].follow_list[k],"$")==0)
				F.follow[i].d=1;
			F.follow[i].size_follow_list++;			
		}
		return;	
	}
	if(start->check==1)
	{
			if(already_present(F.follow[i].follow_list,start->value,F.follow[i].size_follow_list))
				return;		
			if(F.follow[i].size_follow_list==0)
				F.follow[i].follow_list=malloc(sizeof(char*));
			else
				F.follow[i].follow_list=realloc(F.follow[i].follow_list,sizeof(char*)*(F.follow[i].size_follow_list+1));	
		F.follow[i].follow_list[F.follow[i].size_follow_list]=malloc(strlen(start->value)+1);
		strcpy(F.follow[i].follow_list[F.follow[i].size_follow_list],start->value);
		if(strcmp(start->value,"$")==0)
				F.follow[i].d=1;
		F.follow[i].size_follow_list++;	
		return ;
	}
	else
	{
		int t=rank(start->value);			
		for(int k=0;k<F.first[t].size_first_list;k++)
		{
			if(already_present(F.follow[i].follow_list,F.first[t].first_list[k],F.follow[i].size_follow_list))
				continue;
			if(F.follow[i].size_follow_list==0)
				F.follow[i].follow_list=malloc(sizeof(char*));
			else
				F.follow[i].follow_list=realloc(F.follow[i].follow_list,sizeof(char*)*(F.follow[i].size_follow_list+1));	
			F.follow[i].follow_list[F.follow[i].size_follow_list]=malloc(strlen(F.first[t].first_list[k])+1);
			strcpy(F.follow[i].follow_list[F.follow[i].size_follow_list],F.first[t].first_list[k]);
			if(strcmp(F.first[t].first_list[k],"$")==0)
				F.follow[i].d=1;
			F.follow[i].size_follow_list++;			
		}
		if(F.first[t].e==1)
			find_follow(F,i,start->next,l,G);
		return;
	}

		
}
void find_firsts(FirstAndFollow F, int i,grammar G, int it)
{
		
	if(F.first[i].size_first_list!=0)
		return;
	else
	{
		F.first[i].value=nonterminals[i];
		int f=	first_occurence(F.first[i].value,G);
		int l=last_occurence(F.first[i].value,G);
		for(int j=f;j<=l;j++)
		{		
			RHS* start=G.start[j].head;		
			if(start==NULL)
				continue;
			else
			{
				if(start!=NULL && start->check==-1)
				{
					F.first[i].e=1;
					continue;
				}
				if(start->check==1)
				{
					if(already_present(F.first[i].first_list,start->value,F.first[i].size_first_list))
						continue;
					if(F.first[i].size_first_list==0)
						F.first[i].first_list=malloc(sizeof(char*));
					else						
						F.first[i].first_list=realloc(F.first[i].first_list,sizeof(char*)*(F.first[i].size_first_list+1));	
					F.first[i].first_list[F.first[i].size_first_list]=malloc(strlen(start->value)+1);

					strcpy(F.first[i].first_list[F.first[i].size_first_list],start->value);
					F.first[i].size_first_list++;
				}
				else
				{	
					int t;					
					do
					{
						t=rank(start->value);
						find_firsts(F,t,G,0);
						for(int k=0;k<F.first[t].size_first_list;k++)
						{
							if(already_present(F.first[i].first_list,F.first[t].first_list[k],F.first[i].size_first_list))
								continue;
							if(F.first[i].size_first_list==0)
								F.first[i].first_list=malloc(sizeof(char*));
							else
								F.first[i].first_list=realloc(F.first[i].first_list,sizeof(char*)*(F.first[i].size_first_list+1));	
							F.first[i].first_list[F.first[i].size_first_list]=malloc(strlen(F.first[t].first_list[k])+1);
							strcpy(F.first[i].first_list[F.first[i].size_first_list],F.first[t].first_list[k]);
							F.first[i].size_first_list++;				
						}
						start=start->next;
					}while(start!=NULL && F.first[t].e==1 && start->check==0);

					if(start==NULL && F.first[t].e==1)
						F.first[i].e=1;
					if(start!=NULL && start->check==1 && F.first[t].e==1)
					{
						if(already_present(F.first[i].first_list,start->value,F.first[i].size_first_list))
							continue;
						if(F.first[i].size_first_list==0)
							F.first[i].first_list=malloc(sizeof(char*));
						else
							F.first[i].first_list=realloc(F.first[i].first_list,sizeof(char*)*(F.first[i].size_first_list+1));
						F.first[i].first_list[F.first[i].size_first_list]=malloc(strlen(start->value)+1);
						strcpy(F.first[i].first_list[F.first[i].size_first_list],start->value);
						F.first[i].size_first_list++;
					}
				}						
			}
		}	
	}
	return;
}

FirstAndFollow ComputeFirstAndFollowSets(grammar G, FirstAndFollow  F,int it)
{
	for(int i=0;i<G.no_of_nt;i++)
		if(it==0)
			find_firsts(F,i,G,0);

	for(int i=0;i<G.size;i++)
	{		
		RHS* start=G.start[i].head;
		while(start!=NULL)
		{
			if(start->check==1 || start->check==-1)
			{
				start=start->next;
				continue;		
			}
			else 
			{
				int r=rank(start->value);		
				find_follow(F,r,start->next,G.start[i],G);
				start=start->next;		
			}			
		}
	}
	return F;
}


table createParseTable(FirstAndFollow F, table T)
{
	LHS* rules = gram.start;
	int len_rules = gram.size;

	T.head = (LHS* **) malloc(sizeof(LHS**)*T.r);
	for (int i=0; i<T.r; i++) 
        	T.head[i] = (LHS* *)malloc(T.c * sizeof(LHS*)); 

    	for(int i = 0 ; i < T.r ; i++)
    	{
    		for(int j = 0 ; j < T.c ; j++)
    	    		T.head[i][j] = NULL;		
    	}

	for(int i = 0 ; i < len_rules ; i++)
	{
		char * alpha = rules[i].head->value;
		int rank_rule = rank(rules[i].value);
		
		if(strcmp(alpha , "#")==0)
		{
			char**follow_lhs = F.follow[rank_rule].follow_list;
			int len_follow_lhs = F.follow[rank_rule].size_follow_list;
			int follow_$ = F.follow[rank_rule].d;
 
			for(int j = 0 ; j < len_follow_lhs ; j++)
			{
				int temp = terminal_rank(follow_lhs[j]);
				T.head[rank_rule][temp] = &rules[i];
			}


		}
		else if(rules[i].head->check == 0)
		{
			int alpha_index = rank(alpha);
			char **first_alpha = F.first[alpha_index].first_list;
			int len_first_alpha = F.first[alpha_index].size_first_list;
			int first_e = F.first[alpha_index].e;

			char**follow_alpha = F.follow[alpha_index].follow_list;
			int len_follow_alpha = F.follow[alpha_index].size_follow_list;
			int follow_$ = F.follow[alpha_index].d;

			for(int j = 0 ; j < len_first_alpha ; j++)
			{
				int temp = terminal_rank(first_alpha[j]);
				T.head[rank_rule][temp] = &rules[i];

			}

			if(first_e == 1)
			{
				for(int j = 0 ; j < len_follow_alpha ; j++)
				{
					int temp = terminal_rank(follow_alpha[j]);
					T.head[rank_rule][terminal_rank(follow_alpha[j])] = &rules[i];
				}
			}	
			if(first_e == 1 &&  follow_$ == 1)
				T.head[rank_rule][terminal_rank("$")] = &rules[i];				
	
		}
		else if(rules[i].head->check == 1)
		{
			int temp = terminal_rank(alpha);
			T.head[rank_rule][terminal_rank(alpha)] = &rules[i];
		} 
	}		
	return T;
}


void push(char* value , int check, node* ptr) 
{ 
	stack_node* temp; 
    	temp = (stack_node* )malloc(sizeof(stack_node));
  
 	if (temp == NULL)
        	printf("\nHeap Overflow");  
    	temp->value = value;
    	temp->check = check; 
  	temp->ptr = ptr;
    	temp->link = top; 
    	top = temp; 
} 
  

int isEmpty() 
{ 
    	return top == NULL; 
} 
  

stack_node* peek() 
{ 
    	if (!isEmpty()) 
        	return top; 
    	else
        	exit(1); 
} 
  

void pop() 
{ 
    	stack_node* temp; 
  
    	if (top == NULL)
	{ 
        	printf ("\nStack Underflow\n"); 
        	exit(1); 
    	} 
    	else
	{ 
	        temp = top; 
	        top = top->link;   
        	temp->link = NULL; 
	        free(temp); 
    	} 
} 
  
 
void display()  
{ 
    	stack_node* temp; 
    	if (top == NULL)
	{ 
        	printf("\nStack Underflow\n"); 
        	exit(1); 
    	} 
    	else
	{ 
        	temp = top; 
        	while (temp != NULL)
		{ 
            		printf("%s " , temp->value); 
            		temp = temp->link; 
        	} 
        	printf("\n");
    	} 
}


void printExitMsg()
{
	printf("\n\nEXITING THE APPLICATION.\n\nTHANK YOU FOR USING !!!\n\n");
	kill(getpid(), SIGINT);
}
  

node* parse_tree(LHS* lhs , node* n, Lexeme* lex)
{
	int count = 0;
	if(n == NULL)
	{
		p = (parseTree *)malloc(sizeof(parseTree));
		n = (node *)malloc(sizeof(node));
		p -> root = n;
		n -> parent = NULL;
		n -> value = lhs -> value;
		n -> check = 0;
		RHS * rhs = lhs -> head;
		while(rhs!=NULL)
		{	
			rhs = rhs->next;
			count++;
		}
		n->numofchild = count;
		rhs = lhs->head;
		n->children = (node* *)malloc(count * sizeof(node*));
		for(int i = 0 ; i < count ; i++)
		{
			n->children[i] = (node*)malloc(sizeof(node));
			n->children[i]->parent = n;
			n->children[i]->value = rhs->value;
			n->children[i]->check = rhs->check;
			n->children[i]->numofchild = 0;
			if((rhs -> check) == 1)
			{
				n->children[i]->done = 1;
				n->children[i]->lex = lex;
			}
			else
				n->children[i]->done = 0;

			rhs = rhs->next;
		}
	}
	else
	{
		RHS* rhs = lhs->head;
		while(rhs!=NULL)
		{	
			rhs = rhs->next;
			count++;
		}
		n->numofchild = count;
		rhs = lhs->head;
		n->children = (node* *)malloc(count * sizeof(node*));
		for(int i = 0 ; i < count ; i++)
		{
			n->children[i] = (node*)malloc(sizeof(node));
			n->children[i]->parent = n;
			n->children[i]->value = rhs->value;
			n->children[i]->check = rhs->check;
			n->children[i]->numofchild = 0;
			if((rhs -> check) == 1 || (rhs -> check) == -1 )
			{
				n->children[i]->done = 1;
				n->children[i]->lex = lex;
			}
			else
				n->children[i]->done = 0;
			rhs = rhs->next;
		}
	}
	node* temp = n;
	while(temp != NULL)
	{
		if(temp -> done == 1)
		{
			temp = temp->parent;
			continue;
		}

		for(int i = 0; i < temp->numofchild; i++)
		{
			if((temp->children[i]->done) == 0)
				return temp->children[i];

		}
		temp-> done = 1;
		temp = temp->parent;
	}
	return temp;

}


void parseInputSourceCode(FILE * fp1, table T)
{	
	DFA_state = Start;
	buffer1 = getStream(fp1);
	buffer2 = getStream(fp1);
	char *ptr = buffer1;
	Lexeme * l=NULL;
	push(gram.start->value, 0, NULL);
	LHS* lhs;
	RHS* rhs;
	node* n = NULL;
	node* prev = NULL;
	int err_flag = 0 ;
	int lexerr = 0 ;
	Lexeme* prv;
	l=getNextToken(&ptr);
	while (l->lexeme != NULL)
	{
		int l_rank = terminal_rank(l->TOKEN);		
		int stack_rank;
		if(isEmpty())
			return;	
		
		if(terminal_rank(peek()->value)==terminal_rank(l->TOKEN) && peek()->check==1)
		{
			stack_rank = terminal_rank(peek()->value);
			if(terminal_rank(peek()->value) == l_rank)
			{
				if(isEmpty())
					return;	
				peek()->ptr->lex = l;
				l = getNextToken(&ptr);
				if(isEmpty())
					return;
				pop();

				while(l->lexeme!=NULL && strcmp(l->TOKEN,"LEXICAL ERROR")==0)
				{
					printf(YELLOW"LEXICAL ERROR "RESET"at Line Number : %d -> INCORRECT TOKEN '%s'\n\n", l->Line_No, l->lexeme);
					LexErr++;
					prv = l;
					l=getNextToken(&ptr);
					lexerr = 1;
				}			
				if(l->lexeme==NULL)
					return;
				if(l==NULL)
					return;
			} 
			continue;
		}
		if(peek()->check == 0)
		{
			do
			{
				stack_rank = rank(peek()->value);	
				if(stack_rank==-1)
					return;
				lhs = T.head[stack_rank][l_rank];
				if(lhs != NULL)
				{
					
					rhs = lhs->head;
					if(isEmpty())
						return;
					pop();
						
					if(strcmp(rhs->value, "#") == 0)	
					{	
						prev = n;
						n = parse_tree(lhs , n, l);	
						continue;
					}
					
					prev = n;
					n = parse_tree(lhs , n, l);
					
					if(prev==NULL)
						prev = p->root;

					for(int i = prev->numofchild-1 ; i >=0 ; i--)
						push(prev->children[i]->value , prev->children[i]->check , prev->children[i]);

					if(isEmpty())
						return;

				}
				else 
				{

					if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
					{
						printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> Lexeme : %s\n\n",l->Line_No, l->lexeme);
						SynxErr++;
						prv = l;
					}

					if(lexerr == 0)
					{
						int r=rank(peek()->value);					
					}
					else
						lexerr = 0;
					int f=0;
					while(T.head[stack_rank][l_rank]==NULL)
					{
						if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
						{	
							printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> Lexeme : %s\n\n",l->Line_No, l->lexeme);
							SynxErr++;							
							prv = l;
						}	
						
						for(int i=0;i<fo->follow[stack_rank].size_follow_list;i++)
						{
							if(terminal_rank(fo->follow[stack_rank].follow_list[i])==l_rank)
							{	
								if(isEmpty())
									return;
								pop();
								if(!isEmpty() && peek()->check==0)
								{
									n=peek()->ptr;
									
									if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
									{	
										printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> Lexeme : %s\n\n",l->Line_No, l->lexeme);
										SynxErr++;
										prv = l;
									}
								}
								f=1;
							}
						}
						if(f==1)
							break;

						l=getNextToken(&ptr);
						while(l->lexeme!=NULL && strcmp(l->TOKEN,"LEXICAL ERROR")==0)
						{
							if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
							{	
								printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> Lexeme : %s\n\n",l->Line_No, l->lexeme);
								SynxErr++;
								if(strcmp(l->TOKEN,"LEXICAL ERROR")==0)
								{
									LexErr++;
									printf(YELLOW"LEXICAL ERROR "RESET"at Line Number : %d -> INCORRECT TOKEN '%s'\n\n", l->Line_No, l->lexeme);
								}
								prv = l;
							}
							l=getNextToken(&ptr);
						}	
						if(l->lexeme==NULL)
							return;
						if(l==NULL)
						{   
							if(!isEmpty())
							{
								if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
								{		
									printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> Lexeme : %s\n\n",l->Line_No, l->lexeme);
									SynxErr++;
									prv = l;
								}							
							}							
							return;
						}		
						l_rank=terminal_rank(l->TOKEN);
					}	
				}
				if(isEmpty())
					return;
			}while(!isEmpty() && n != NULL && peek()->check == 0);
		}
		if(!isEmpty() && peek()->check==1 && terminal_rank(peek()->value)!=terminal_rank(l->TOKEN))
		{
			
			if(prv == NULL || prv->Line_No != l->Line_No || prv->TOKEN != l->TOKEN)
			{	
				printf(RED"SYNTAX ERROR "RESET"at Line Number : %d -> TERMINAL MISMATCH. Expected TOKEN : %s , Received Token : %s\n\n", l->Line_No, peek()->value, l->TOKEN);
				SynxErr++;
				prv = l;
			}			
			pop();
		}
		if(isEmpty() )
			return;	
	}
	DFA_state = Start;
	line_count = 1;
	return;
}


void printParseTree(node* n)
{
    if(n==NULL)
		return;
    if(n->numofchild!=0)
    	printParseTree(n->children[0]);
	if(n->check == 1 && n->lex != NULL)
	{
		if(strcmp(n->lex->TOKEN , "NUM")==0)
			printf("  %-30s %-20d %-20s %-20d %-40s %-20s %-30s \n" , n->lex->lexeme , n->lex->Line_No , n->lex->TOKEN , n->lex->Value.val , n->parent->value , "YES" , "----" ); 
		else if(strcmp(n->lex->TOKEN , "RNUM")==0)
			printf("  %-30s %-20d %-20s %-20lf %-40s %-20s %-30s \n" , n->lex->lexeme , n->lex->Line_No , n->lex->TOKEN , n->lex->Value.real_val , n->parent->value , "YES" , "----"); 
		else
			printf("  %-30s %-20d %-20s %-20s %-40s %-20s %-30s \n" , n->lex->lexeme , n->lex->Line_No , n->lex->TOKEN , "----" , n->parent->value , "YES" , "----"); 
	}
	else if(n->check == 0)
		printf("  %-30s %-20s %-20s %-20s %-40s %-20s %-30s \n" , "----" , "----" , "----" , "----", n->parent==NULL?"ROOT":n->parent->value , "NO", n->value ); 
    
    	for(int i = 1 ; i < n->numofchild; i++)
        	printParseTree(n->children[i]);
	return;
}



void parser()
{
	FILE * fp = fopen("Grammar.txt", "r");
	if(fp==NULL)
	{
		perror("Grammar file not found");
		exit(0);
	}
	char str[100];
	char* token;
	char* rest;
	char* lhsvalue;
	LHS* lhs;
	RHS* rhs;
	int i = 0;
	char prev_lhs_value[50] = "";
	int j =0;
	char temp[50] = "";
	gram.no_of_nt=0;

	while(fscanf(fp, "%[^\n]\n", str) != EOF)
	{ 
		rest = str;
		token = strtok_r(rest," ", &rest);
		if(i == 0)
		{
			gram.start=malloc(sizeof(LHS));
			gram.start[i].value=malloc(sizeof(char)*(strlen(token)+1));
			strcpy(gram.start[i].value, token);
			gram.start[i].head = NULL;
			gram.size=1;
		}
		else
		{
			LHS* ptr=gram.start;			
			gram.start=realloc(gram.start,sizeof(LHS)*(i+1));
			gram.start[i].value=malloc(sizeof(char)*(strlen(token)+1));
			strcpy(gram.start[i].value, token);
			gram.start[i].head = NULL;
			gram.size++;
		}

		populate_nt(&gram);
		token = strtok_r(rest,"-", &rest);
		char str[]="#";
		if(!strcmp(token, str))
		{
			gram.start[i].e = 1;
			rhs = (RHS *)malloc(sizeof(RHS));
			gram.start[i].head = rhs;
			rhs->value=malloc(sizeof(char)*(strlen(token)+1));
			strcpy(rhs -> value, token);
			rhs -> next = NULL;
			i++;
			continue;
		}

		if((i != 0) && (!strcmp(gram.start[i-1].value, gram.start[i].value)) && (gram.start[i-1].e == 1))
			gram.start[i].e = 1;

		else
			gram.start[i].e = 0;

		rhs = (RHS *)malloc(sizeof(RHS));
		gram.start[i].head = rhs;
		rhs->value=malloc(sizeof(char)*(strlen(token)+1));
		strcpy(rhs -> value, token);
		rhs -> next = NULL;
		RHS * prev = rhs;
		 
		while ((token = strtok_r(rest, "-", &rest)))
		{

				rhs = (RHS *)malloc(sizeof(RHS));
				prev -> next = rhs;
				rhs->value=malloc(sizeof(char)*(strlen(token)+1));
				strcpy(rhs -> value, token);
				rhs -> next = NULL;
				prev = rhs;
    		}
		i++;
	}

	find_nt(gram );
	int sum=0;
	F.first=malloc(sizeof(First)*(gram.no_of_nt));
	F.follow=malloc(sizeof(Follow)*(gram.no_of_nt));

	for(int i=0;i<gram.no_of_nt;i++)
	{
		F.first[i].size_first_list=0;
		F.follow[i].size_follow_list=0;
		F.first[i].e=0;
		F.follow[i].d=0;
	}

	hash_nt_ptr=malloc(sizeof(hash_nt*)*factor_nt);
	for(int i=0;i<factor_nt;i++)
		hash_nt_ptr[i]=NULL;

	fill_hash_nt_table();
	F.no_of_firsts=gram.no_of_nt;
	F.no_of_follows=gram.no_of_nt;
	F.follow[0].follow_list=malloc(sizeof(char*));
	F.follow[0].follow_list[0]=malloc(sizeof(char)*2);
	F.follow[0].follow_list[0][0]='$';
	F.follow[0].follow_list[0][1]=0;
	F.follow[i].d=1;
	F.follow[0].size_follow_list++;
	int it=0;
	F=ComputeFirstAndFollowSets(gram,F,it);
	it++;
	F=ComputeFirstAndFollowSets(gram,F,it);
	F=ComputeFirstAndFollowSets(gram,F,it);
	F=ComputeFirstAndFollowSets(gram,F,it);
	int size=0;
	for(int i=0;i<gram.size;i++)
	{
		RHS* start=gram.start[i].head;
		while(start!=NULL)
		{
			if(start->check==1)
				size=fill_terminal(start->value,size);
			start=start->next;
		}
	}
	size=fill_terminal("$",size);
	no_of_terminals=size;

	hash_t_ptr=malloc(sizeof(hash_t*)*factor_t);
	for(int i=0;i<factor_t;i++)
		hash_t_ptr[i]=NULL;

	fill_hash_t_table();	
	fo=&F;
	T.r = gram.no_of_nt;
	T.c = no_of_terminals;
	T = createParseTable(F,  T);
	T = createParseTable(F,  T);
	printf("\n\n*************************** FIRST and FOLLOW SETS AUTOMATED ***************************\n\n");
}


void print_First()
{
	printf("--------------------------------------Firsts-----------------------------------\n");	
	for(int i=0;i<gram.no_of_nt;i++)
	{
		printf("%s  ",gram.start[i].value);
		for(int j=0;j<fo->first[i].size_first_list;j++)
		{
			if(j==0)
				printf("%s",fo->first[i].first_list[j]);
			else
				printf("-%s",fo->first[i].first_list[j]);		
		}
		printf("\n");
	}
	printf("--------------------------------------EndFirsts--------------------------------\n");
}


void print_Follow()
{
	printf("--------------------------------------Follows-----------------------------------\n");	
	for(int i=0;i<gram.no_of_nt;i++)
	{
		printf("%s  ",gram.start[i].value);
		for(int j=0;j<fo->follow[i].size_follow_list;j++)
		{
			if(j==0)
				printf("%s",fo->follow[i].follow_list[j]);
			else
				printf("-%s",fo->follow[i].follow_list[j]);		
		}
		printf("\n");
	}
	printf("--------------------------------------EndFollows-----------------------------------\n");
}


void freetree(node *n)
{
	if(n==NULL)
		return;

	for(int i=0; i<n->numofchild; i++)
		freetree(n->children[i]);
	free(n);
	return;
}

