/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/


#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "assembler.h"

int line_count;
int buf_count;
int DFA_state;
int Printst;
int no_of_terminals;
int factor_nt;
int factor_t;

int SynxErr = 0;
int LexErr = 0;
int SemErr = 0;
int symind = 0;

int op3 = 0;
int asop = 0;
int numAST = 0;
int numParseTree = 0;

extern int switch_label;
extern int while_label;
extern int for_label;
extern int relationalop_label;
extern int logicalop_label;
extern int floatnum;
extern int func_space;

extern int boolprint;
extern int intarrprint;
extern int boolarrprint;
extern int realarrprint;
extern int boolelemprint;
extern int getbool;
extern int getintarr;
extern int getboolarr;
extern int getrealarr;


void menuOptions(int option, FILE* sourceFile, char* filename, char* asmfile)
{
	line_count = 1;
	DFA_state = Start;
	switch(option)
	{
		case 1:
			printf("\n\nLIST OF TOKENS\n");
			printf("*************************************************************************************\n");
			Token_Print(sourceFile);
			return;

		case 2:
			fclose(sourceFile);
			char x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			sourceFile = fopen(filename, "r");
			printf("\n\nPARSING THE INPUT\n\n");
			printf("*************************************************************************************\n");
			parseInputSourceCode(sourceFile, T);
			printf("\nPrinting the Parse Tree in the In-Order Traversal : Left Child, Parent, Remaining Children\n\n"); 
			printf("\n  %-30s %-20s %-20s %-20s %-40s %-20s %-30s \n", "CURRENT NODE LEXEME", "LINE NUMBER", "TOKEN NAME", "VALUE OF NUMBER", "PARENT NODE SYMBOL", "IS LEAF", "NODE SYMBOL");
			for(int i=0; i<200; i++)
				printf("_");
			printf("\n");
			printParseTree(p->root);
			printf("\n\nPARSE TREE CONSTRUCTED\n\n");
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n\n");	
			fclose(sourceFile);
			FILE* InputFile = fopen(filename, "r+");
			fseek(InputFile, -1, SEEK_END);
    			int position=ftello(InputFile);
    			ftruncate(fileno(InputFile),position);
			fclose(InputFile);
			return;

		case 3: 
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile = fopen(filename, "r+");
			fseek(IpFile, -1, SEEK_END);
    			int postion=ftello(IpFile);
    			ftruncate(fileno(IpFile),postion);
			fclose(IpFile);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("ABSTRACT SYNTAX TREE CONSTRUCTED\n");
			printf("\nPrinting the Abstract Syntax Tree in the Pre-Order Traversal : Parent, Left Child, Remaining Children\n\n");
			printf("\n  %-30s %-20s %-20s %-20s %-40s %-20s %-30s \n", "CURRENT NODE LEXEME", "LINE NUMBER", "TOKEN NAME", "VALUE OF NUMBER", "PARENT NODE SYMBOL", "IS LEAF", "NODE SYMBOL");
			for(int i=0; i<200; i++)
					printf("_");
			printf("\n");
			PrintAST(asttreee->root);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n");		
			return;

		case 4:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			int sizeastnode = sizeof(ast_node);
			int sizeparsenode = sizeof(node);
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile1 = fopen(filename, "r+");
			fseek(IpFile1, -1, SEEK_END);
    			int postion1 = ftello(IpFile1);
    			ftruncate(fileno(IpFile1), postion1);
    			sizeastnode -= 26;
			fclose(IpFile1);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n");
			CountParseTree(p->root);
			CountAST(asttreee->root);
			int parsesize = sizeparsenode*numParseTree;
			int astsize = sizeastnode*numAST;
			printf("\nNumber of Parse Tree Nodes = %d\t\tAllocated Memory for Parse Tree Nodes = %d bytes\n", numParseTree, parsesize);
			printf("Number of Abstract Syntax Tree Nodes = %d\tAllocated Memory for Abstract Syntax Tree Nodes = %d bytes\n",numAST, astsize);
			printf("Compression percentage : %lf\n\n", (((double)parsesize - astsize)/parsesize)*100.0);
			numAST = 0;
			numParseTree = 0;
			return;

		case 5:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile2 = fopen(filename, "r+");
			fseek(IpFile2, -1, SEEK_END);
    			int postion2 = ftello(IpFile2);
    			ftruncate(fileno(IpFile2),postion2);
			fclose(IpFile2);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n");
			printf("PERFORMING SEMANTIC ANALYSIS OF THE SOURCE CODE\n\n");
			st_node* root = create_st(asttreee);
			if(SemErr!=0)
			{
				printf(RED"%d Semantic Errors Found\n"RESET, SemErr);
				SemErr = 0;
			}
			else
				printf(GREEN"No Semantic Errors Found\n"RESET);
			printf("\nPRINTING THE SYMBOL TABLE\n");
			printf("************************************************************************************\n\n");
			printSymbolTable(root);
			symind = 0;
			return;
		
		case 6:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile3 = fopen(filename, "r+");
			fseek(IpFile3, -1, SEEK_END);
    			int postion3 = ftello(IpFile3);
    			ftruncate(fileno(IpFile3), postion3);
			fclose(IpFile3);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n");
			printf("\nPERFORMING SEMANTIC ANALYSIS OF THE SOURCE CODE\n\n\n");
			st_node* root2 = create_st(asttreee);
			if(SemErr!=0)
			{
				printf(RED"%d Semantic Errors Found\n"RESET, SemErr);
				SemErr = 0;
			}
			else
				printf(GREEN"No Semantic Errors Found\n"RESET);
			printf("\n");
			print_activation_record(root2);
			printf("\n");
			return;
			
		case 7:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile12 = fopen(filename, "r+");
			fseek(IpFile12, -1, SEEK_END);
    			int postion12 = ftello(IpFile12);
    			ftruncate(fileno(IpFile12), postion12);
			fclose(IpFile12);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n\n");
			printf("PERFORMING SEMANTIC ANALYSIS OF THE SOURCE CODE\n\n\n");
			st_node* root12 = create_st(asttreee);
			if(SemErr!=0)
			{
				printf(RED"%d Semantic Errors Found\n"RESET, SemErr);
				SemErr = 0;
			}
			else
				printf(GREEN"No Semantic Errors Found\n"RESET);
			printArray(root12);
			return;

		case 8:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);	
				clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile4 = fopen(filename, "r+");
			fseek(IpFile4, -1, SEEK_END);
    			int position4 = ftello(IpFile4);
    			ftruncate(fileno(IpFile4), position4);
			fclose(IpFile4);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n\n");
			printf("PERFORMING SEMANTIC ANALYSIS OF THE SOURCE CODE\n\n\n");
			st_node* root3 = create_st(asttreee);
			if(SemErr!=0)
			{
				printf(RED"%d Semantic Errors Found\n"RESET, SemErr);
				SemErr = 0;
			}
			else
				printf(GREEN"No Semantic Errors Found\n"RESET);
			 	end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
			printf("\n\nTOTAL CPU TIME : %lf Clock Ticks\t\tTOTAL CPU TIME IN SECONDS: %lf s\n\n", total_CPU_time, total_CPU_time_in_seconds);
			return;

		case 9:
			fclose(sourceFile);
			x='$';
			sourceFile = fopen(filename, "a"); 
			fputc(x,sourceFile);
			fclose(sourceFile);
                start_time = clock();
			sourceFile = fopen(filename, "r");
			printf("\nPARSING THE INPUT\n");
			printf("*************************************************************************************\n\n");
			parseInputSourceCode(sourceFile, T);
			fclose(sourceFile);
			FILE* IpFile5 = fopen(filename, "r+");
			fseek(IpFile5, -1, SEEK_END);
    			int position5 = ftello(IpFile5);
    			ftruncate(fileno(IpFile5), position5);
			fclose(IpFile5);
			if(SynxErr!=0 || LexErr!=0)
			{
				printf("\033[01;31m%d Syntactic Errors Found and %d Lexical Errors Found\n\033[0m\n", SynxErr, LexErr);
				SynxErr = 0;
				LexErr = 0;
				return;
			}
			else
				printf("\033[1;32mNo Syntactic and Lexical Errors Found\n\033[0m\n");
			printf("\nAST CREATION\n\n");
			printf("*************************************************************************************\n\n");
			asttreee = ast_creation(p);
			printf("\nABSTRACT SYNTAX TREE CONSTRUCTED\n\n\n");
			printf("PERFORMING SEMANTIC ANALYSIS OF THE SOURCE CODE\n\n\n");
			st_node* root4 = create_st(asttreee);
			if(SemErr!=0)
			{
				printf(RED"%d Semantic Errors Found\n"RESET, SemErr);
				SemErr = 0;
				return;
			}
			else
				printf(GREEN"No Semantic Errors Found\n"RESET);
			CodeGenStart(asttreee, root4, asmfile);
			printf(GREEN"\n\nCode Compiles Successfully..........\n\n"RESET);
			printf("\nGenerated the NASM Assembly Level Code of your Program in file %s\n\n", asmfile);
				end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
			printf("\n\nTOTAL CPU TIME : %lf Clock Ticks\t\tTOTAL CPU TIME IN SECONDS: %lf s\n\n", total_CPU_time, total_CPU_time_in_seconds);
			return;

		default:
			printf("\n\nPLEASE ENTER A VALID OPTION BETWEEN 0 AND 9\n");
			return;
	}
	return;
}


int main(int argc, char* argv[])
{
	line_count = 1;
	buf_count = 0;
	DFA_state = Start;
	Printst = Initial;
	no_of_terminals = 0;
	factor_nt = 317;
	factor_t = 127;

	//FIRST MESSAGE
	printf("\nLEVEL 4 Compiler: Symbol Table / Type Checking / Semantic Rules Modules Work / Handled Static and Dynamic Arrays in Type Checking and Code Generation\n");

	ht = initialize_hash_table();
	ast_ht = ast_initialization();
	parser();
	int incorrect_attempts = 0;
	char filename[30];
	char* correctfile;
	char *asmfile;
	if(argv[2]==NULL || argv[1]==NULL)
		asmfile = "code.asm";
	else
		asmfile = argv[2];
	do{
		if(incorrect_attempts>0)
		{
			printf("\n*************************** ERROR Opening the Source file (FILE NOT FOUND/ FILE NAME MORE THAN 30 CHARACTERS) ***************************\n\n\n");
			printf("Try Different Name: ");
			scanf("%s", filename);
			sourceCode = fopen(filename, "r");
			incorrect_attempts++;
		}
		else
		{
			sourceCode = fopen(argv[1], "r");
			incorrect_attempts++;
		}
	}while(sourceCode==NULL || strlen(filename)>30);

	incorrect_attempts--;

	if(incorrect_attempts==0)
		correctfile = argv[1];
	else
		correctfile = filename;

	printf("\n******************* OPENED FILE \"%s\"*******************\n\n", correctfile);
	fclose(sourceCode);

	int option;
	do{	
		printf("\n*************************************************************************************");
		printf("\nMAIN MENU\n\n");
		printf("0 -> EXIT\n");
		printf("1 -> Printing the token list generated by the Lexer\n");
		printf("2 -> Parsing the input by the Parser and produce Parse Tree\n");
		printf("3 -> Creating the Abstract Syntax Tree\n");
		printf("4 -> Calculating the Compression Percentage\n");
		printf("5 -> Printing the Symbol Table\n");
		printf("6 -> Printing the Activation Record Size (Stack Size) for each Function\n");
		printf("7 -> Listing all the Static and Dynamic Arrays\n");
		printf("8 -> Verifying the Semantic Correctness and Calculating Compile Time\n");
		printf("9 -> Generating the Assembly Code (LINUX Based NASM v2.14.02)\n\n");
		printf("Select an option: ");
		scanf("%d", &option);
		sourceCode = fopen(correctfile, "r");
		if(option==0)
			break;	
		menuOptions(option, sourceCode, correctfile, asmfile);
	}while(1);
	fclose(sourceCode);
	printExitMsg();	
	return 0;
}
	
