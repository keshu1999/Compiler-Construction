/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include "lexer.h"

static inline void* hash_table(char* p, char* t, struct h_table* ht)
{
	int length_of_string=0;
	int len=0;
	while (p[length_of_string])
		length_of_string++;
	while(t[len])
		len++;
	size_t siz = length_of_string+1;
	size_t siz1 = len+1;
	char* pat = (char *)malloc(sizeof(char)*siz);
	char* pat1 = pat;
	while(*p)
	{
		*pat1 = *p;
		p++;
		pat1++;
	}
	char* tok = (char *)malloc(sizeof(char)*siz1);
	char* q = tok;
	while(*t)
	{
		*q = *t;
		t++;
		q++;
	}
	*q='\0';
	*pat1='\0';

	struct keywords* keyword= (struct keywords* ) malloc(sizeof(struct keywords));
	keyword->pattern=pat;
	keyword->token=tok;
	keyword->next=NULL;

	int a=0;
	for(int i=0; i<length_of_string; i++)
		a = a+keyword->pattern[i];
	int index_of_htable= ((a+5)*a)%table_size;
	ht->size++;

	if(ht->arr[index_of_htable]==NULL)
		ht->arr[index_of_htable] = keyword;
	else
	{
		struct keywords* next_keyword = ht->arr[index_of_htable];

		while(next_keyword->next!=NULL)
			next_keyword=next_keyword->next;
		next_keyword->next=keyword;
	}
}


struct h_table* initialize_hash_table()
{
	struct h_table* ht;
	ht = (struct h_table *)malloc(sizeof(struct h_table));
	ht->size=0;
	FILE *tokens = fopen("Keywords.txt", "r");
	if(tokens==NULL)
	{	
		perror("Keywords file Not Found");
		exit(1);
	}
	char str[30], str2[30];
	while(fscanf(tokens, "%s %s\n", str, str2) != EOF)
		hash_table(str,str2,ht);
	hash_table("$", "$", ht);
	printf("\n\n*************************** KEYWORDS INITIALIZED ***************************\n");
	fclose(tokens);
	return ht;
}


char* find_token(char* pattern)
{
	int a = 0;
	int length_of_string=0;
	char *p=pattern;
	while(*pattern)
	{
		length_of_string++;
		*pattern++;
	}
	for(int i=0; i<length_of_string; i++)
		a = a + p[i];
	int index_of_htable = ((a+5)*a)%table_size;

	struct keywords *next_keyword = ht->arr[index_of_htable];
	char *x, *y;
	while(next_keyword!=NULL && next_keyword->next!=NULL)
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
		if(!(*(const unsigned char *)x-*(const unsigned char *)y))
			return next_keyword->token;
		else
			next_keyword = next_keyword->next;
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
			return next_keyword->token;
		else
			return "ID";
	}
	else
		return "ID";

}


static inline double get_value_double(char* input)
{
	double ans = 0.0;
	int len = 0;
	while (input[len])
		len++;
	char* a= (char *)malloc(sizeof(char)*len);
	int i;
	for(i=0; i<len&&(input[i]!='e'||input[i]!='E'); i++)
		a[i]=input[i];
	a[i]='\0';
	i++;
	int flag=0;
	if(input[i]=='+')
		i++;
	else if(input[i]=='-')
	{
		i++;
		flag=1;
	}
	ans=atof(a);
	int j;
	for(j=0; j<len-i; j++,i++)
		a[j]=input[i];
	a[j]='\0';
	int power=atoi(a);
	for(int k=0; k<power; k++)
	{
		if(flag==1)
			ans=(double)ans/(double)10;
		else if(flag==0)
			ans=ans*10;
	}
	return ans;
}


char* getStream(FILE *fp)
{
	buf_count++;
	int charCount = 0;
	char ch;
	char* charbuff = (char *)malloc(sizeof(char)*(FILEBYTES+1));
	for(int i=0; i<FILEBYTES; i++)
		charbuff[i] = '\0';
	while((!feof(fp))&&(charCount!=FILEBYTES))
	{
		ch = fgetc(fp);
		if(ch!=EOF)
			charbuff[charCount++] = ch;
		else
			break;
	}
	charbuff[charCount] = '\0';
	if(buf_count>=2)
		free(free_ptr[(buf_count-2)%10]);
	return charbuff;
}


Lexeme* getNextToken(char** ptr)
{
	Lexeme* mylexeme = (Lexeme *)malloc(sizeof(Lexeme));
	int len=0;
	len = strlen(buffer1);
	char c, *tok = (char *)malloc(sizeof(char)*50);
	for(int i=0; i<50; i++)
		tok[i] = '\0';
	int idlen = 0;
	while(!(**ptr=='\0'&& *buffer2=='\0'))
	{
		if(*ptr <= buffer1+len-1)
		{
			switch(DFA_state)
			{
				case Start:
					c = **ptr;
					++*ptr;
					if(c==' '||c=='\t')
						break;

					else if(c=='\n')
					{
						line_count++;
						break;
					}

					else if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
						DFA_state = Alpha1;

					else if(c>='0'&&c<='9')
						DFA_state = Num1;

					else if(c=='*')
						DFA_state = C1;

					else if(c=='<')
						DFA_state = Les1;

					else if(c=='>')
						DFA_state = Grt1;

					else if(c==':')
						DFA_state = Col1;

					else if(c=='.')
						DFA_state = Ran1;

					else if(c=='+')
						DFA_state = Plus;

					else if(c=='-')
						DFA_state = Minus;

					else if(c=='/')
						DFA_state = Divide;

					else if(c==';')
						DFA_state = Semcol;

					else if(c=='=')
						DFA_state = Equal1;

					else if(c=='!')
						DFA_state = NotEqual1;

					else if(c==',')
						DFA_state = Com;

					else if(c=='[')
						DFA_state = SqbO;

					else if(c==']')
						DFA_state = SqbC;

					else if(c=='(')
						DFA_state = BracO;

					else if(c==')')
						DFA_state = BracC;

					else if(c=='$')
					{	
						tok[idlen++] = c;
						tok[idlen]='\0';
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = find_token(tok);
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}

					else if(c=='{')
						DFA_state = Start2;

					else if(c=='}')
						DFA_state = End2;

					else
					{
						DFA_state = Error;
						break;
					}
					tok[idlen++] = c;
				break;


				case Alpha1:
					c = **ptr;
					++*ptr;
					if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_')
					{
						DFA_state = Alpha1;
						tok[idlen++] = c;
					}
					else
					{
						DFA_state = Alpha2;
						--*ptr;
					}
				break;

				case Alpha2:
					if(idlen<=20)
					{
						tok[idlen] = '\0';
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = find_token(tok);
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}
					else
					{
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "LEXICAL ERROR";
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}

				case Num1:
					c = **ptr;
					++*ptr;
					if(c>='0'&&c<='9')
					{
						DFA_state = Num1;
						tok[idlen++] = c;
					}
					else if(c=='.')
					{
						DFA_state = RNum1;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Num2;
					}
				break;

				case Num2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "NUM";
					mylexeme->Value.val = atoi(tok);
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case RNum1:
					c = **ptr;
					++*ptr;
					if(c=='.')
						DFA_state = RangRet;
					else if(c>='0'&&c<='9')
					{
						DFA_state = RNum2;
						tok[idlen++] = c;
					}
					else if(!(c>='0'&&c<='9'))
					{
						--*ptr;
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "LEXICAL ERROR";
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}
				break;

				case RangRet:
					idlen--;
					tok[idlen]='\0';
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "NUM";
					mylexeme->Value.val = atoi(tok);
					mylexeme->Line_No = line_count;
					DFA_state = ROP;
				return mylexeme;

				case ROP:
					mylexeme->lexeme = "..";
					mylexeme->TOKEN = "RANGEOP";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case RNum2:
					c = **ptr;
					++*ptr;
					if(c>='0'&&c<='9')
					{
						DFA_state = RNum2;
						tok[idlen++] = c;
					}
					else if((c=='e'||c=='E') && tok[idlen-1]>='0' && tok[idlen-1]<='9')
					{
						DFA_state = RNum3;
						tok[idlen++] = c;
					}
					else if(!(tok[idlen-1]>='0' && tok[idlen-1]<='9'))
					{
						--*ptr;
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "LEXICAL ERROR";
						mylexeme->Value.val = 6;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}
					else
					{
						--*ptr;
						DFA_state = RNum6;
					}
				break;


				case RNum3:
					c = **ptr;
					++*ptr;
					if(c>='0'&&c<='9')
					{
						DFA_state = RNum5;
						tok[idlen++] = c;
					}
					else if(c=='+'||c=='-')
					{
						DFA_state = RNum4;
						tok[idlen++] = c;
					}
					else if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_')
					{	char t=tok[idlen-1];
						--*ptr;
						tok[idlen-1]='\0';
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "RNUM";
						mylexeme->Value.real_val = get_value_double(tok);
						mylexeme->Line_No = line_count;
						if(t=='e')
							DFA_state = RNum7;
						else
							DFA_state = RNum8;
						return mylexeme;

						
					}
					else
						DFA_state = Error;
				break;

				case RNum7:
					tok[idlen++]='e';
					DFA_state = Alpha1;
				break;

				case RNum8:
					tok[idlen++]='E';
					DFA_state = Alpha1;
				break;
				
				case RNum4:
					c = **ptr;
					++*ptr;
					if(c>='0'&&c<='9')
					{
						DFA_state = RNum5;
						tok[idlen++] = c;
					}
					else
					{
						DFA_state = Error;
						--*ptr;
					}
				break;

				case RNum5:
					c = **ptr;
					++*ptr;
					if(c>='0'&&c<='9')
					{
						DFA_state = RNum5;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = RNum6;
					}
				break;

				case RNum6:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "RNUM";
					mylexeme->Value.real_val = get_value_double(tok);
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;


				case C1:
					c = **ptr;
					++*ptr;
					if(c=='*')
					{
						DFA_state = C2;
						idlen--;
						tok[idlen] = '\0';
					}
					else
					{
						DFA_state = Mul;
						--*ptr;
					}
				break;

				case C2:
					c = **ptr;
					++*ptr;
					if(c=='\n')
						line_count++;
					if(c=='*')
						DFA_state = C3;
					else
						DFA_state = C2;
				break;

				case C3:
					c = **ptr;
					++*ptr;
					if(c=='\n')
						line_count++;
					if(c=='*')
						DFA_state = C4;
					else
						DFA_state = C2;
				break;

				case C4:
					DFA_state = Start;
				break;

				case Mul:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "MUL";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Les1:
					c = **ptr;
					++*ptr;
					if(c=='<')
					{
						DFA_state = Les2;
						tok[idlen++] = c;
					}
					else if(c=='=')
					{
						DFA_state = Les3;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Les4;
					}
				break;

				case Les2:
					c = **ptr;
					++*ptr;
					if(c=='<')
					{
						DFA_state = Les5;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Les6;
					}
				break;

				case Les3:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "LE";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Les4:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "LT";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Les5:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "DRIVERDEF";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Les6:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "DEF";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Grt1:
					c = **ptr;
					++*ptr;
					if(c=='>')
					{
						DFA_state = Grt2;
						tok[idlen++] = c;
					}
					else if(c=='=')
					{
						DFA_state = Grt3;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Grt4;
					}
				break;

				case Grt2:
					c = **ptr;
					++*ptr;
					if(c=='>')
					{
						DFA_state = Grt5;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Grt6;
					}
				break;

				case Grt3:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "GE";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Grt4:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "GT";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Grt5:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "DRIVERENDDEF";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Grt6:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "ENDDEF";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Col1:
					c = **ptr;
					++*ptr;
					if(c=='=')
					{
						DFA_state = Col2;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						DFA_state = Col3;
					}
				break;

				case Col2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "ASSIGNOP";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Col3:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "COLON";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Ran1:
					c = **ptr;
					++*ptr;
					if(c=='.')
					{
						DFA_state = Ran2;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "LEXICAL ERROR";
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;
					}
				break;

				case Ran2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "RANGEOP";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Plus:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "PLUS";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Minus:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "MINUS";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Divide:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "DIV";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Semcol:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "SEMICOL";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Equal1:
					c = **ptr;
					++*ptr;
					if(c=='=')
					{
						DFA_state = Equal2;
						tok[idlen++] = c;
					}
					else
					{
						--*ptr;
						mylexeme->lexeme = tok;
						mylexeme->TOKEN = "LEXICAL ERROR";
						mylexeme->Value.val = 0;
						mylexeme->Line_No = line_count;
						DFA_state = Start;
						return mylexeme;

					}
				break;

				case Equal2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "EQ";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case NotEqual1:
					c = **ptr;
					++*ptr;
					if(c=='=')
					{
						DFA_state = NotEqual2;
						tok[idlen++] = c;
					}
				break;

				case NotEqual2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "NE";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Com:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "COMMA";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case SqbO:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "SQBO";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case SqbC:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "SQBC";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case BracO:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "BO";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case BracC:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "BC";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case Error:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "LEXICAL ERROR";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
					return mylexeme;
				break;

				case Start2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "START";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;

				case End2:
					mylexeme->lexeme = tok;
					mylexeme->TOKEN = "END";
					mylexeme->Value.val = 0;
					mylexeme->Line_No = line_count;
					DFA_state = Start;
				return mylexeme;
			}
		}
		else
		{
			if(strlen(buffer2)==0 && **ptr=='\n')
			{
				buffer1 = buffer2;
				*ptr = buffer1;
				buffer2 = getStream(sourceCode);
				len = 4;
			}
			else
			{
				buffer1 = buffer2;
				*ptr = buffer1;
				buffer2 = getStream(sourceCode);
				len = strlen(buffer1);
			}
		}
	}
	mylexeme->lexeme = NULL;
	return mylexeme;
}


void Token_Print(FILE* fp)
{
	buffer1 = getStream(fp);
	buffer2 = getStream(fp);
	char *ptr = buffer1;
	Lexeme* lex = NULL;
	printf("   %-30s      %-50s %-30s\n", "LINE NUMBER", "LEXEME", "TOKEN NAME");
	for(int i=0; i<=100; i++)
		printf("_");
	printf("\n");
	do{
		free(lex);
		lex = getNextToken(&ptr);
		if((lex->lexeme!=NULL)&&(strcmp(lex->TOKEN, "$")))
		{
			if(strcmp(lex->TOKEN, "LEXICAL ERROR")==0)
			{
				printf("\t%-30d %-50s", lex->Line_No, lex->lexeme);
				printf("\033[01;31m %-30s\n\033[0m", lex->TOKEN);
			}			
			else			
				printf("\t%-30d %-50s %-30s\n", lex->Line_No, lex->lexeme, lex->TOKEN);
		}		
		if((lex->lexeme != NULL) && strcmp(lex->TOKEN, "LEXICAL ERROR")==0)
			LexErr++;
	}while(lex->lexeme != NULL);
	DFA_state = Start;
	free(buffer1);
	free(buffer2);
	line_count = 1;
	if(LexErr!=0)
		printf("\033[01;31m\n%d Lexical Errors Found\n\033[0m", LexErr);
	else
		printf("\033[1;32m\nNo Lexical Errors Found\n\033[0m");
	LexErr = 0;
	return;
}


void Toktime(FILE* fp)
{
	buffer1 = getStream(fp);
	buffer2 = getStream(fp);
	char *ptr = buffer1;
	Lexeme* lex = NULL;
	do{
		free(lex);
		lex = getNextToken(&ptr);
	}while(lex->lexeme != NULL);
	DFA_state = Start;
	free(buffer1);
	free(buffer2);
	line_count = 1;
}


void removeComments(FILE *testCaseFile)
{
	char ch;
	while(!feof(testCaseFile))
	{
		ch = fgetc(testCaseFile);
		if(ch!=EOF)
		{
			switch(Printst)
			{
				case Initial:
					if(ch=='*')
						Printst = Com1;
					else if(ch=='$')
						break;
					else
						printf("%c", ch);
				break;

				case Com1:
					if(ch=='*')
						Printst = Com2;
					else
					{
						printf("*%c", ch);
						Printst = Initial;
					}
				break;

				case Com2:
					if(ch=='*')
						Printst = Com3;
					else if(ch=='\n' || ch==' ' || ch=='\t')
						printf("%c", ch);
				break;

				case Com3:
					if(ch=='*')
						Printst = Initial;
					else if(ch=='\n')
					{
						Printst = Com2;
						printf("\n");
					}
					else if(ch==' ' || ch=='\t')
					{
						printf("%c", ch);
						Printst = Com2;
					}
					else
						Printst = Com2;
				break;
			}
		}
		else
			break;
	}
	return;
}

