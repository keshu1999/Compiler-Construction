/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "ast.h"

static inline void* add_hash_table_ast(char * p,struct ast_h_table* ht)
{
	int length_of_string=0;
	int len=0;
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
	struct ast_keywords* keyword=(struct ast_keywords*) malloc(sizeof(struct ast_keywords));
	keyword->pattern=pat;
	keyword->next=NULL;
	int a=0;
	for (int i=0;i<length_of_string;i++)
		a=a+keyword->pattern[i];
	int index_of_htable=(a*a+5*a)%SIZEOF_AST_HASH_TABLE;
	ht->size++;
	if(ht->arr[index_of_htable]==NULL)
		ht->arr[index_of_htable]=keyword;
	else
	{
		struct ast_keywords * next_keyword=ht->arr[index_of_htable];
	
		while(next_keyword->next!=NULL)
			next_keyword=next_keyword->next;
		next_keyword->next=keyword;
	}
}


struct ast_h_table* ast_initialization()
{
	struct ast_h_table* ht;
	ht = (struct ast_h_table *) malloc(sizeof(struct ast_h_table));
	for(int i=0; i<SIZEOF_AST_HASH_TABLE; i++)
		ht->arr[i] = NULL;
	FILE *AST = fopen("ASTWords.txt", "r");
	if(AST==NULL)
	{
		perror("AST Keywords file Not Found");
		exit(1);
	}
	ht->size=0;
	char str[30];
	while(fscanf(AST, "%s\n", str) != EOF)
		add_hash_table_ast(str, ht);
	printf("\n\n************************* AST KEYWORDS INITIALIZED *************************\n");
	fclose(AST);
	return ht;
}


int ast_find_token(char* pattern)
{	
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
	int index_of_htable=(a*a+5*a)%SIZEOF_AST_HASH_TABLE;
	struct ast_keywords * next_keyword=ast_ht->arr[index_of_htable];
	char * x;
	char*y;
	while(next_keyword!=NULL&&next_keyword->next!=NULL)
	{	
		x=next_keyword->pattern;
		y=p;
		while(*x)
		{
			if(*x!=*y)
				break;
			x++;
			y++;
		}
		if(!(*(const unsigned char*)x-*(const unsigned char*)y))
			return 1;
		else
			next_keyword=next_keyword->next;
	}
	if(next_keyword!=NULL)
	{
		x=next_keyword->pattern;
		y=p;
		while(*x)
		{		
			if(*x!=*y)
				break;
			x++;
			y++;
		}
		if(!(*x-*y))
			return 1;
		else
			return 0;
	}
	else
		return 0;
	
}


ast_node* make_node(int argc, ast_node** argv, char* name)
{
	ast_node* newnode = (ast_node *)malloc(sizeof(ast_node));
	newnode->child = argv[0];
	int i;
	for(i = 0 ; i < argc ; i++)
	{
		argv[i]->parent = newnode;
		if(i != argc-1)		
			argv[i]->next = argv[i+1];
		else
			argv[i]->next = NULL;
	}
	newnode->childsize = i;
	for(i = 0 ; i < argc ; i++)
		argv[i]->prev = argv[i-1];
	strcpy(newnode->name.nt_name , name);
	newnode->tag = 0;
	return newnode;
}

ast_node* make_leaf(Lexeme* lex)
{
	ast_node* newnode = (ast_node *)malloc(sizeof(ast_node));
	newnode->parent = NULL;
	newnode->child = NULL;
	newnode->next = NULL;
	newnode->prev = NULL;
	newnode->name.l = lex;
	newnode->tag = 1;
	newnode->childsize = 0;
	return newnode;

}

//AST Stack functions

int ast_isEmpty()
{
	return ast_top == NULL ? 1 : 0;
}

void ast_push(node* ptr, int ind)
{
	ast_stack_node* stack_node = (ast_stack_node* )malloc(sizeof(ast_stack_node));
	stack_node->pnode = ptr;
	stack_node->index = ind;
	if(ast_isEmpty())
	{
		ast_top = stack_node;
		stack_node -> down = NULL;
	}
	else
	{
		stack_node -> down = ast_top;
		ast_top = stack_node;
	}
}


ast_stack_node* ast_peek()
{
	if(ast_isEmpty())
		printf("Stack Underflow\n");
	return ast_top;
}


void ast_pop()
{
	ast_stack_node* temp; 
    	if (ast_top == NULL)
	{	 
        	printf ("\nStack Underflow\n"); 
        	exit(1); 
    	} 
    	else
	{ 
	        temp = ast_top; 
	        ast_top = ast_top -> down;   
        	temp -> down = NULL; 
	        free(temp); 
    	} 
}


void ast_increment()
{
	if(ast_isEmpty())
	{
		printf("Stack Underflow\n");
		exit(1);
	}
	ast_peek() -> index++;
	if((ast_peek() -> index) > ((ast_peek() -> pnode -> numofchild) - 1))
		ast_peek()->index = -1;
}


ast_tree* ast_creation(parseTree *p)
{
	ast_tree* astree = malloc(sizeof(ast_tree));  
	node * current = p->root;
	int null_prod = 0, req_child;
	node * parent;

	while((current != NULL) || (!ast_isEmpty()))
	{
	
		if(current != NULL) 
		{
			if((current -> check) == 0)
			{
				ast_push(current, 0);
				current = current -> children[0];
				continue;
			}

			if((current -> check) == 1)
			{
				if(ast_find_token(current->lex->TOKEN))				/* Check if it is a meaningful terminal*/
					current -> addr = make_leaf(current -> lex);

				else
					current -> addr = NULL;
				ast_increment();						// Increment index of child in the stack;
				current = NULL;
				continue;
			}

			if((current -> check) == -1)
			{
				current -> addr = NULL;
				parent = ast_peek() -> pnode;
				parent -> addr = NULL;
				ast_increment();
				current = NULL;
				continue;
			}
		}

		else 
		{
			parent = ast_peek() -> pnode;
			req_child = ast_peek() -> index;
			if(null_prod == 1)
			{
				req_child = -1;
				null_prod = 0;
			}
			if(req_child == -1 || req_child==parent->numofchild) 			// All children of parent traversed
			{
				int count = 0;
				int ind = 0;
				for(int j = 0; j < parent -> numofchild; j++) 			//Find count of all meaningful children of the parent
				{								// Check if numofchild is 0 for a null production rule
					if(parent -> children[j] -> addr != NULL)
					{
						count++;
						ind = j;
					}
				}
				if(count == 1) 							// If only 1 meaningful dont makenode, just pass address
					parent -> addr = parent -> children[ind] -> addr;

				else if(count == 0);

				else 								// Have to do makenode here
				{
					ast_node** list = (ast_node**)malloc(sizeof(ast_node*) * count);
					ind = 0;
					for(int j = 0; j < parent -> numofchild; j++)
					{
						if((parent -> children[j] -> addr) != NULL)
							list[ind++] = parent -> children[j] -> addr;
					}
					parent -> addr = make_node(ind, list, parent -> value); // After creating list of meaningful children, call makenode()
					astree -> root = parent -> addr;
				}
				ast_pop();
				ast_increment();
				req_child = ast_peek() -> index;
				parent = ast_peek() -> pnode;
				while(req_child == -1 || req_child == parent->numofchild)
				{
					int count = 0;
					int ind = 0;
					for(int j = 0; j < parent -> numofchild; j++)
					{
						if(parent -> children[j] -> addr != NULL)
						{
							count++;
							ind = j;
						}
					}


					if(count == 1)
						parent -> addr = parent -> children[ind] -> addr;
					else
					{
						ast_node** list = (ast_node** )malloc(sizeof(ast_node*) * count);
						ind = 0;
						for(int j = 0; j < parent -> numofchild; j++)
						{
							if((parent -> children[j] -> addr) != NULL)
								list[ind++] = parent -> children[j] -> addr;
						}
						parent -> addr = make_node(ind, list, parent -> value);
						astree -> root = parent -> addr;
					}
					ast_pop();
					if(ast_isEmpty())
					{
						current = NULL;
						break;
					}
					ast_increment();
					req_child = ast_peek() -> index;
					parent = ast_peek() -> pnode;
				}
			}
			if(ast_isEmpty())
				break;
			current =  parent -> children[req_child];
		}
	}
	if(strcmp(astree->root->name.nt_name, "program")!=0)
	{
		ast_node** lst = (ast_node** )malloc(sizeof(ast_node*));
		lst[0] = astree->root;
		astree->root = make_node(1, lst, "program");
	}
	astree->root->parent=NULL;
	astree->root->next = NULL;
	astree->root->prev = NULL;
	return astree;
}


void PrintAST(ast_node *root)
{
	if(root==NULL)
		return;
	if(root->tag==1)
	{
		if(strcmp(root->name.l->TOKEN , "NUM")==0)
			printf("  %-30s %-20d %-20s %-20d %-40s %-20s %-30s \n" , root->name.l->lexeme , root->name.l->Line_No , root->name.l->TOKEN , root->name.l->Value.val , root->parent->name.nt_name , "YES" , "----" ); 
		else if(strcmp(root->name.l->TOKEN , "RNUM")==0)
			printf("  %-30s %-20d %-20s %-20lf %-40s %-20s %-30s \n" , root->name.l->lexeme , root->name.l->Line_No , root->name.l->TOKEN , root->name.l->Value.real_val , root->parent->name.nt_name , "YES" , "----"); 
		else
			printf("  %-30s %-20d %-20s %-20s %-40s %-20s %-30s \n" , root->name.l->lexeme , root->name.l->Line_No , root->name.l->TOKEN , "----" , root->parent->name.nt_name , "YES" , "----"); 
	}
	else if(root->tag==0)
		printf("  %-30s %-20s %-20s %-20s %-40s %-20s %-30s \n" , "----" , "----" , "----" , "----", root->parent==NULL?"ROOT":root->parent->name.nt_name , "NO", root->name.nt_name); 
	
	PrintAST(root->child);
	PrintAST(root->next);
	return;
}


void CountParseTree(node *root)
{
	if(root==NULL)
		return;
	numParseTree++;
	for(int i=0; i<root->numofchild; i++)
		CountParseTree(root->children[i]);
}


void CountAST(ast_node *root)
{
	if(root==NULL)
		return;
	numAST++;
	CountAST(root->child);
	CountAST(root->next);
}

