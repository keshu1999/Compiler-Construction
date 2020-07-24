/*
* GROUP NUMBER : 16
* GANDHI ATITH NIKESHKUMAR	: 2017A7PS0062P 
* BURHAN BOXWALLA			: 2017A7PS0097P
* KESHAV SHARMA				: 2017A7PS0140P
* SHRAY MATHUR				: 2017A7PS1180P
* RAJ SANJAY SHAH			: 2017A7PS1181P
*/
#include  "assembler.h"

void initializeLables()
{
	switch_label=0;
	while_label=0;
	for_label=0;
	relationalop_label=0;
	logicalop_label=0;
	floatnum=0;
	func_space=0;

	boolprint = 0;
	intarrprint = 0;
	boolarrprint = 0;
	realarrprint = 0;
	boolelemprint = 0;
	getbool = 0;
	getintarr = 0;
	getboolarr = 0;
	getrealarr = 0;
}


FILE *fasm;
void CodeGenStart(ast_tree* tree, st_node* root, char* file)
{
	fasm = fopen(file, "w");
	initializeLables();
	inherited f;
	st_node* main = lookup_function_main_scope_table("driver", root, 1);
	PrintTemplateStart();	
	fprintf(fasm, "\t\tPUSH	RBP\n");
	fprintf(fasm, "\t\tMOV 	RBP,	RSP\n");
	if(main->maxoffset%16==0)
		fprintf(fasm, "\t\tSUB	RSP,	%d\n", main->maxoffset);
	else
		fprintf(fasm, "\t\tSUB	RSP,	%d\n", (1+main->maxoffset/16)*16);
	CodeGen(tree->root,f, root);	
	PrintTemplateEnd();
	fclose(fasm);
}



void CodeGen(ast_node* root,inherited f, st_node* st_root)
{
	if(root==NULL)
		return;
	if(root->tag==0 && !strcmp(root->name.nt_name,"module"))
		return;
	if(root->tag==0 && !strcmp(root->name.nt_name,"followingArithExp"))
	{
		if(root->childsize==2)
		{
			if( !strcmp(root->child->next->ty->node_type,"INTEGER"))
			{				
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EBX,	%d\n",root->child->next->name.l->Value.val);

				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))	
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RBP-%d]\n",root->child->next->symbol->offset);	

				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");						
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RDI]\n");
					fprintf(fasm, "\t\tPOP		RAX\n");
				}	
				else if(root->child->next->tag==0)
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");					
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV	 EBX,	EAX\n");
					fprintf(fasm, "\t\tPOP	RAX\n");	
				}
				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"PLUS"))
					fprintf(fasm, "\t\tADD	EAX,	EBX\n");

				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"MINUS"))
					fprintf(fasm, "\t\tSUB	EAX,	EBX\n");
				fprintf(fasm, "\n");
			}
			else if( !strcmp(root->child->next->ty->node_type,"REAL"))
			{				
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"RNUM"))
				{
					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,root->child->next->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM1, 	DWORD[FLOAT%d]\n",floatnum);
				}
						
				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))	
					fprintf(fasm, "\t\tMOVSS	 	XMM1, 	DWORD[RBP - %d]\n",root->child->next->symbol->offset);	
				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
				{
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOVSS		XMM1, 	[RDI]\n");
				}	
				else if(root->child->next->tag==0)
				{
					CodeGen(root->child->next, f, st_root);	
					fprintf(fasm, "\t\tMOVSS	 	XMM1, 	XMM0\n");		
				}
			}
		}
		else
		{
			if( !strcmp(root->child->next->next->ty->node_type,"INTEGER"))	
			{
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EBX,	%s\n",root->child->next->name.l->lexeme);

				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RBP-%d]\n",root->child->next->symbol->offset);

				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var") )
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");					
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RDI]\n");
					fprintf(fasm, "\t\tPOP	RAX\n");
				}
				else if(root->child->next->tag==0 )
				{
					fprintf(fasm, "\t\tPUSH		RAX\n");				
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV	EBX,	EAX\n");
					fprintf(fasm, "\t\tPOP	RAX\n");	
									
				}
				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"PLUS"))
					fprintf(fasm, "\t\tADD	EAX,	EBX\n");

				else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"MINUS"))
					fprintf(fasm, "\t\tSUB	EAX,	EBX\n");
				fprintf(fasm, "\n");

				CodeGen(root->child->next->next, f, st_root);				
			}
			else if( !strcmp(root->child->next->next->ty->node_type,"REAL"))	
			{
				CodeGen(root->child->next->next, f, st_root);	
				fprintf(fasm, "sub rsp, 24 \n");
				fprintf(fasm, "movss dword[rsp], xmm1 \n");
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"RNUM"))
				{
					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,root->child->next->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
				}
				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOVSS 	XMM0, 	DWORD[RBP- %d]\n",root->child->next->symbol->offset);
				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var") )
				{
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "MOVSS XMM0, [RDI] \n");
				}
				else if(root->child->next->tag==0 )
				{
					CodeGen(root->child->next, f, st_root);					
				}
				fprintf(fasm, "movss xmm1, dword[rsp] \n");
				fprintf(fasm, "add rsp, 24 \n");

				if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"PLUS"))
					fprintf(fasm, "addss xmm0, xmm1 \n");
				if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"MINUS"))
					fprintf(fasm, "subss xmm0, xmm1 \n");
				fprintf(fasm, "movss xmm1, xmm0 \n");
								
			}	
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"followingTerm"))
	{
		if(root->childsize==2)
		{
			if( !strcmp(root->child->next->ty->node_type,"INTEGER"))
			{				
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EBX,	%s\n",root->child->next->name.l->lexeme);

				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))	
					fprintf(fasm,"\t\tMOV 	EBX,	DWORD [RBP-%d]\n",root->child->next->symbol->offset);	

				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
				{
					fprintf(fasm, "\t\tPUSH		RAX\n");				
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RDI]\n");
					fprintf(fasm, "\t\tPOP	RAX\n");
				}	
				else if(root->child->next->tag==0 )
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");	
					CodeGen(root->child->next, f, st_root);	
					fprintf(fasm, "\t\tMOV	EBX,	EAX\n");
					fprintf(fasm, "\t\tPOP	RAX\n");		
				}
				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"MUL"))
					fprintf(fasm, "\t\tIMUL	EBX\n");

				else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"DIV"))
				{
					fprintf(fasm, "\t\tMOV 	RDX,	0\n");
					fprintf(fasm, "\t\tIDIV	EBX\n");
				}
				fprintf(fasm, "\n");

			}
			else if( !strcmp(root->child->next->ty->node_type,"REAL"))
			{				
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"RNUM"))
				{

					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,root->child->next->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM1, 	DWORD[FLOAT%d]\n",floatnum);
				}
				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))	
					fprintf(fasm, "MOVSS XMM1, DWORD[rbp - %d]\n",root->child->next->symbol->offset);	
				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
				{
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "MOVSS XMM1, DWORD[RDI] \n");
				}	
				else if(root->child->next->tag==0 )
				{
					CodeGen(root->child->next, f, st_root);	
					fprintf(fasm, "MOVSS XMM1, XMM0 \n");		
				}
			}
		}
		else
		{
			if( !strcmp(root->child->next->next->ty->node_type,"INTEGER"))	
			{
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EBX,	%s\n",root->child->next->name.l->lexeme);

				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RBP-%d]\n",root->child->next->symbol->offset);

				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var") )
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");					
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RDI]\n");
					fprintf(fasm, "\t\tPOP	RAX\n");
				}
				else if(root->child->next->tag==0  )
				{
					fprintf(fasm, "\t\tPUSH 	RAX\n");					
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "\t\tMOV	EBX,	EAX\n");
					fprintf(fasm, "\t\tPOP	RAX\n");					
				}

				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"MUL"))
					fprintf(fasm, "\t\tIMUL	EBX\n");

				if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"DIV"))
				{
					fprintf(fasm, "\t\tMOV 	RDX,	0\n");
					fprintf(fasm, "\t\tIDIV	EBX\n");
				}
				fprintf(fasm, "\n");

				CodeGen(root->child->next->next, f, st_root);			
			}
			else if( !strcmp(root->child->next->next->ty->node_type,"REAL"))	
			{

				CodeGen(root->child->next->next, f, st_root);

				fprintf(fasm, "sub rsp, 24 \n");
				fprintf(fasm, "movss dword[rsp], xmm1 \n");
				if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"RNUM"))
				{
/*					printf(" ax, %s \n",root->child->next->name.l->lexeme);*/
					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm ,"FLOAT%d 	DD 		%s\n", floatnum ,root->child->next->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
				}
				else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))
					fprintf(fasm, "MOVSS XMM0, DWORD[rbp- %d] \n",root->child->next->symbol->offset);
				else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var") )
				{
					CodeGen(root->child->next, f, st_root);
					fprintf(fasm, "MOVSS XMM0, DWORD[RDI] \n");
				}
				else if(root->child->next->tag==0  )
					CodeGen(root->child->next, f, st_root);					

				fprintf(fasm, "movss xmm1, dword[rsp] \n");
				fprintf(fasm, "add rsp, 24 \n");
				if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"MUL"))
				{
					fprintf(fasm, "mulss xmm0, xmm1\n");
				}
				if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"DIV"))
					fprintf(fasm, "divss xmm0, xmm1 \n");

				fprintf(fasm, "movss xmm1, xmm0 \n");			
			}	
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"arithmeticExpr"))
	{
		if( !strcmp(root->child->next->ty->node_type,"INTEGER"))
		{
			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"NUM"))
				fprintf(fasm, "\t\tMOV 	EAX,	%s\n",root->child->name.l->lexeme);

			else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"ID") )
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RBP-%d]\n",root->child->symbol->offset);

			else if(root->child->tag==0 && !strcmp(root->child->name.nt_name,"var") )
			{
				CodeGen(root->child, f, st_root);
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RDI]\n");
			}
			else if(root->child->tag==0 )
				CodeGen(root->child, f, st_root);
			fprintf(fasm, "\n");
		
			CodeGen(root->child->next, f, st_root);
		}
		else if( !strcmp(root->child->next->ty->node_type,"REAL"))
		{
			CodeGen(root->child->next, f, st_root);

			fprintf(fasm, "sub rsp, 24 \n");
			fprintf(fasm, "movss dword[rsp], xmm1 \n");
			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"RNUM"))
			{
				floatnum++;
				fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
				fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,root->child->name.l->lexeme);
				fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");				
				fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
			}
			else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"ID") )
				fprintf(fasm, "MOVSS XMM0, DWORD[rbp- %d] \n",root->child->symbol->offset);
			else if(root->child->tag==0 && !strcmp(root->child->name.nt_name,"var") )
			{
				CodeGen(root->child, f, st_root);
				fprintf(fasm, "MOVSS XMM0, DWORD[RDI] \n");
				
			}
			else if(root->child->tag==0 )
			{
				CodeGen(root->child, f, st_root);
				
			}
			fprintf(fasm, "movss xmm1, dword[rsp] \n");
			fprintf(fasm, "add rsp, 24 \n");						
			if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"PLUS"))
				fprintf(fasm, "addss xmm0, xmm1 \n");
			if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"MINUS"))
				fprintf(fasm, "subss xmm0, xmm1 \n");
		}
		return;
	
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"term"))
	{
		
		if( !strcmp(root->child->next->ty->node_type,"INTEGER"))
		{						
			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"NUM"))
				fprintf(fasm, "\t\tMOV 	EAX,	%s\n",root->child->name.l->lexeme);

			else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"ID") )
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RBP-%d]\n",root->child->symbol->offset);
				
			else if(root->child->tag==0 && !strcmp(root->child->name.nt_name,"var") )
			{
				CodeGen(root->child, f, st_root);
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RDI]\n");
			}
			else if(root->child->tag==0 )
				CodeGen(root->child, f, st_root);
			fprintf(fasm, "\n");						
			
			CodeGen(root->child->next, f, st_root);
		}
		else if( !strcmp(root->child->next->ty->node_type,"REAL"))
		{
			
			
			CodeGen(root->child->next, f, st_root);
			

			fprintf(fasm, "sub rsp, 24 \n");
			fprintf(fasm, "movss dword[rsp], xmm1 \n");
			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"RNUM"))
			{
				floatnum++;
				fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
				fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,root->child->name.l->lexeme);
				fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
				fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
			}
			else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"ID") )
			{				
				fprintf(fasm, "MOVSS XMM0, DWORD[rbp- %d] \n",root->child->symbol->offset);
				
			}
			else if(root->child->tag==0 && !strcmp(root->child->name.nt_name,"var") )
			{
				CodeGen(root->child, f, st_root);
				fprintf(fasm, "MOVSS XMM0, DWORD [RDI] \n");
			}
			else if(root->child->tag==0 )
			{
				CodeGen(root->child, f, st_root);
			}

			fprintf(fasm, "movss xmm1, dword[rsp] \n");
			fprintf(fasm, "add rsp, 24 \n");	
			if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"MUL"))
				fprintf(fasm, "mulss xmm0, xmm1 \n");
			if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"DIV"))			
			{
/*				printf("mov dx, 0 \n");				*/
				fprintf(fasm, "divss xmm0, xmm1 \n");
			}
		}
		return;
	
	}

	if(root->tag==0 && !strcmp(root->name.nt_name,"assignmentStmt"))
	{
		if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"lvalueIDStmt"))
		{
			ast_node* en=root->child->next->child->next;
			if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID") && en->symbol->arr==1)
			{
				fprintf(fasm, "\t\tMOV	RAX,	QWORD [RBP-%d]\n",en->symbol->offset);
				fprintf(fasm, "\t\tMOV	QWORD[RBP-%d],	RAX\n", root->child->symbol->offset);
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", en->symbol->offset-8);
				fprintf(fasm, "\t\tMOV 	BX, 	WORD [RBP-%d]\n", root->child->symbol->offset-8);
				fprintf(fasm, "\t\tCMP	AX, 	BX\n");
				fprintf(fasm, "\t\tJNE	ARRAY_EQUAL_ERROR\n");
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", en->symbol->offset-10);
				fprintf(fasm, "\t\tMOV 	BX, 	WORD [RBP-%d]\n", root->child->symbol->offset-10);
				fprintf(fasm, "\t\tCMP	AX, 	BX\n");
				fprintf(fasm, "\t\tJNE	ARRAY_EQUAL_ERROR\n");	
			}						
			else if(!strcmp(en->ty->node_type,"INTEGER"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EAX,	%s\n",en->name.l->lexeme);

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RBP-%d]\n",en->symbol->offset);
				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RDI]\n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);

				fprintf(fasm, "\t\tMOV	DWORD [RBP-%d],	EAX\n", root->child->symbol->offset);	
			}
			else if(!strcmp(en->ty->node_type,"REAL"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"RNUM"))
				{
					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,en->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
				}
				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "MOVSS XMM0, DWORD[rbp- %d] \n",en->symbol->offset);
				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "MOVSS XMM0, DWORD[RDI] \n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);			
				fprintf(fasm, "MOVSS DWORD[rbp - %d], XMM0 \n", root->child->symbol->offset);		
			}
			else if(!strcmp(en->ty->node_type,"BOOLEAN"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"TRUE"))
					fprintf(fasm, "\t\tMOV 	AL, 	1\n");

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"FALSE"))
					fprintf(fasm, "\t\tMOV 	AL, 	0\n");

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RBP-%d]\n",en->symbol->offset);

				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RDI]\n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);			
				fprintf(fasm, "\t\tMOV	BYTE [RBP-%d],	AL\n", root->child->symbol->offset);	
			}
		}
		else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"lvalueARRStmt"))
		{
			CodeGen(root->child->next, f, st_root);		
			fprintf(fasm, "\t\tPUSH	RDI\n");
			ast_node* en=root->child->next->child->next->next;						
			if(!strcmp(en->ty->node_type,"INTEGER"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"NUM"))
					fprintf(fasm, "\t\tMOV 	EAX,	%s\n",en->name.l->lexeme);

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RBP-%d]\n",en->symbol->offset);

				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RDI]\n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);

				fprintf(fasm, "\t\tPOP	RDI\n");		
				fprintf(fasm, "\t\tMOV	DWORD [RDI],	EAX\n");		
			}
			else if(!strcmp(en->ty->node_type,"REAL"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"RNUM"))
				{
/*					fprintf(fasm, "mov ax, %s \n",en->name.l->lexeme);*/
					floatnum++;
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
					fprintf(fasm, "FLOAT%d 	DD 		%s\n", floatnum ,en->name.l->lexeme);
					fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n");					
					fprintf(fasm, "\t\tMOVSS	XMM0, 	DWORD[FLOAT%d]\n",floatnum);
				}
				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "MOVSS XMM0, DWORD[rbp- %d] \n",en->symbol->offset);
				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "MOVSS XMM0, DWORD[RDI] \n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);	
				fprintf(fasm, "pop RDI\n");		
				fprintf(fasm, "movss [RDI], xmm0 \n");		
			}
			else if(!strcmp(en->ty->node_type,"BOOLEAN"))
			{
				if(en->tag==1 && !strcmp(en->name.l->TOKEN,"TRUE"))
					fprintf(fasm, "\t\tMOV 	AL, 	1\n");

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"FALSE"))
					fprintf(fasm, "\t\tMOV 	AL, 	0\n");

				else if(en->tag==1 && !strcmp(en->name.l->TOKEN,"ID"))
					fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RBP-%d]\n",en->symbol->offset);

				else if(en->tag==0 && !strcmp(en->name.nt_name,"var") )
				{
					CodeGen(en, f, st_root);
					fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RDI]\n");
				}
				else if(en->tag==0)
					CodeGen(en, f, st_root);	

				fprintf(fasm, "\t\tPOP	RDI\n");		
				fprintf(fasm, "\t\tMOV	BYTE [RDI],	AL\n");		
			}
		}
		fprintf(fasm, "\n");
		return;	
	}
	
	if(root->tag==0 && !strcmp(root->name.nt_name, "lvalueARRStmt"))
	{
		ast_node* ArrID = root->parent->child;
		ast_node* ArrIndex = root->child;
		char* Array_type = ArrID->symbol->type;
		if(!strcmp(ArrIndex->name.l->TOKEN,"NUM"))
		{
			if(ArrID->symbol->end_arr_dec==1 && ArrID->symbol->start_arr_dec==1)
			{
				int element_size = 0;
				if(!strcmp(Array_type, "BOOLEAN"))
					element_size = 1;
				else if(!strcmp(Array_type, "INTEGER"))
					element_size = 4;	
				else if(!strcmp(Array_type, "REAL"))
					element_size = 8;

				fprintf(fasm, "\t\tMOV 	RSI,	0\n");
				fprintf(fasm, "\t\tMOV 	SI, 	%d\n", ArrIndex->name.l->Value.val);
				fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);			
				fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);
				fprintf(fasm, "\t\tADD	RDI,	RSI\n");
				fprintf(fasm, "\n");
			}
			else
			{
				int element_size = 0;
				if(!strcmp(Array_type, "BOOLEAN"))
					element_size = 1;
				else if(!strcmp(Array_type, "INTEGER"))
					element_size = 4;	
				else if(!strcmp(Array_type, "REAL"))
					element_size = 8;
				fprintf(fasm, "\t\tMOV 	RSI,	0\n");
				fprintf(fasm, "\t\tMOV 	SI, 	%d\n", ArrIndex->name.l->Value.val);
				fprintf(fasm, "\t\tCMP 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tJL	SEGFAULT_ERROR\n");
				fprintf(fasm, "\t\tCMP 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 10);
				fprintf(fasm, "\t\tJG 	SEGFAULT_ERROR\n");
				fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);
				fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);
				fprintf(fasm, "\t\tADD 	RDI, 	RSI\n");
				fprintf(fasm, "\n");
			}
		}
		else if(!strcmp(ArrIndex->name.l->TOKEN, "ID"))
		{
			int element_size = 0;
			if(!strcmp(Array_type, "BOOLEAN"))
				element_size = 1;
			else if(!strcmp(Array_type, "INTEGER"))
				element_size = 4;	
			else if(!strcmp(Array_type, "REAL"))
				element_size = 8;
			fprintf(fasm, "\t\tMOV 	RSI,	0\n");
			fprintf(fasm, "\t\tMOV 	SI, 	[RBP-%d]\n", ArrIndex->symbol->offset);
			fprintf(fasm, "\t\tCMP 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
			fprintf(fasm, "\t\tJL 	SEGFAULT_ERROR\n");
			fprintf(fasm, "\t\tCMP 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 10);
			fprintf(fasm, "\t\tJG 	SEGFAULT_ERROR\n");	
			fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
			fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);
			fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);
			fprintf(fasm, "\t\tADD 	RDI, 	RSI\n");
			fprintf(fasm, "\n");
		}
		return;
	}

	if(root->tag==0 && !strcmp(root->name.nt_name,"boolTerm"))
	{
		relationalop_label++;	
		int rl=relationalop_label;	
		ast_node* ae2 = root->child->next->child->next;	
		if(!strcmp(ae2->ty->node_type,"INTEGER"))
		{
			if(ae2->tag==1 && !strcmp(ae2->name.l->TOKEN,"NUM"))
				fprintf(fasm, "\t\tMOV 	EBX,	%s\n",ae2->name.l->lexeme);

			else if(ae2->tag==1 && !strcmp(ae2->name.l->TOKEN,"ID") )
				fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RBP-%d]\n",ae2->symbol->offset);

			else if(ae2->tag==0 && !strcmp(ae2->name.nt_name,"var") )
			{
				CodeGen(ae2, f, st_root);
				fprintf(fasm, "\t\tMOV 	EBX,	DWORD [RDI]\n");
			}
			else if(ae2->tag==0)
			{
				CodeGen(ae2, f, st_root);
				fprintf(fasm, "\t\tMOV	EBX,	EAX\n");
			}
			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tPUSH 	RBX\n");

			ast_node* ae1 = root->child;	
			if(ae1->tag==1 && !strcmp(ae1->name.l->TOKEN,"NUM"))
				fprintf(fasm, "\t\tMOV 	EAX,	%s\n",ae1->name.l->lexeme);

			else if(ae1->tag==1 && !strcmp(ae1->name.l->TOKEN,"ID") && !strcmp(ae1->symbol->type,"INTEGER"))
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RBP-%d]\n",ae1->symbol->offset);
			
			else if(ae1->tag==0 && !strcmp(ae1->name.nt_name,"var") && !strcmp(ae1->child->symbol->type,"INTEGER"))
			{
				CodeGen(ae1, f, st_root);
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD [RDI]\n");
			}
			else if(ae1->tag==0)
				CodeGen(ae1, f, st_root);

			fprintf(fasm, "\t\tPOP	RBX\n\n");

			ast_node* ao = root->child->next->child;
			if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"LT"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJL 	RELATIONAL_LT_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_LT_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_LT_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_LT_END_%d:\n",rl);
			}	
			else if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"LE"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJLE 	RELATIONAL_LE_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_LE_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_LE_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_LE_END_%d:\n",rl);
			}
			else if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"GT"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJG 	RELATIONAL_GT_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_GT_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_GT_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_GT_END_%d:\n",rl);
			}	
			else if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"GE"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJGE 	RELATIONAL_GE_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_GE_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_GE_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_GE_END_%d:\n",rl);
			}	
			else if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"EQ"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJZ 	RELATIONAL_EQ_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_EQ_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_EQ_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_EQ_END_%d:\n",rl);
			}	
			else if(ao->tag==1 && !strcmp(ao->name.l->TOKEN,"NE"))
			{
				fprintf(fasm, "\t\tCMP	EAX, 	EBX\n");
				fprintf(fasm, "\t\tJNZ 	RELATIONAL_NE_%d\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
				fprintf(fasm, "\t\tJMP 	RELATIONAL_NE_END_%d\n",rl);
				fprintf(fasm, "RELATIONAL_NE_%d:\n",rl);
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");
				fprintf(fasm, "RELATIONAL_NE_END_%d:\n",rl);
			}
		}
		fprintf(fasm, "\n");
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"followingBool"))
	{
		if(root->childsize==2)
		{
			if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"TRUE"))
				fprintf(fasm, "\t\tMOV 	BL, 	1\n");

			else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"FALSE"))
				fprintf(fasm, "\t\tMOV 	BL, 	0\n");

			else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))	
				fprintf(fasm, "\t\tMOV 	BL,	BYTE [RBP-%d]\n",root->child->next->symbol->offset);	

			else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
			{
				CodeGen(root->child->next, f, st_root);
				fprintf(fasm, "\t\tMOV 	BL,	BYTE [RDI]\n");
			}	
			else if(root->child->next->tag==0 )
			{
				CodeGen(root->child->next, f, st_root);	
				fprintf(fasm, "\t\tMOV 	BL, 	AL\n");		
			}
			fprintf(fasm, "\n");
		}
		else if(root->childsize==3)
		{
			logicalop_label++;	
			int ll= logicalop_label;		
			if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"TRUE"))
				fprintf(fasm, "\t\tMOV 	AL, 	1\n");

			else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"FALSE"))
				fprintf(fasm, "\t\tMOV 	AL, 	0\n");
			
			else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID") )
				fprintf(fasm, "\t\tMOV 	AL,	BYTE [RBP-%d]\n",root->child->next->symbol->offset);
			
			else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var") )
			{
				CodeGen(root->child->next, f, st_root);
				fprintf(fasm, "\t\tMOV 	AL,	BYTE [RDI]\n");
			}
			else if(root->child->next->tag==0 )
				CodeGen(root->child->next, f, st_root);

			if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"AND"))			
			{
				fprintf(fasm, "\t\tCMP	AL, 	0\n");
				fprintf(fasm, "\t\tJZ 	LOGICAL_AND_END_%d\n",ll);				
				CodeGen(root->child->next->next, f, st_root);	
				fprintf(fasm, "\t\tMOV 	AL, 	BL\n");			
				fprintf(fasm, "LOGICAL_AND_END_%d:\n",ll);
			}
			if(root->child->next->next->child->tag==1 && !strcmp(root->child->next->next->child->name.l->TOKEN,"OR"))
			{
				fprintf(fasm, "\t\tCMP	AL, 	1\n");
				fprintf(fasm, "\t\tJZ 	LOGICAL_OR_END_%d\n",ll);				
				CodeGen(root->child->next->next, f, st_root);	
				fprintf(fasm, "\t\tMOV 	AL, 	BL\n");			
				fprintf(fasm, "LOGICAL_OR_END_%d:\n",ll);			
			}			
			fprintf(fasm, "\t\tMOV 	BL, 	AL\n");
			fprintf(fasm, "\n");
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"arithmeticOrBooleanExpression"))
	{
		logicalop_label++;	
		int ll=	logicalop_label;					
		if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"TRUE"))
			fprintf(fasm, "\t\tMOV 	AL, 	1\n");

		else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"FALSE"))
			fprintf(fasm, "\t\tMOV 	AL, 	0\n");

		else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"ID") )
			fprintf(fasm, "\t\tMOV 	AL,	BYTE [RBP-%d]\n",root->child->symbol->offset);

		else if(root->child->tag==0 && !strcmp(root->child->name.nt_name,"var") )
		{
			CodeGen(root->child, f, st_root);
			fprintf(fasm, "\t\tMOV 	AL,	BYTE [RDI]\n");
		}
		else if(root->child->tag==0)
			CodeGen(root->child, f, st_root);					

		if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"AND"))
		{
			fprintf(fasm, "\t\tCMP	AL, 	0\n");
			fprintf(fasm, "\t\tJZ 	LOGICAL_AND_END_%d\n",ll);				
			CodeGen(root->child->next, f, st_root);		
			fprintf(fasm, "\t\tMOV 	AL, 	BL\n");			
			fprintf(fasm, "LOGICAL_AND_END_%d:\n",ll);
		}
		if(root->child->next->child->tag==1 && !strcmp(root->child->next->child->name.l->TOKEN,"OR"))
		{
			fprintf(fasm, "\t\tCMP	AL, 	1\n");
			fprintf(fasm, "\t\tJZ 	LOGICAL_OR_END_%d\n",ll);				
			CodeGen(root->child->next, f, st_root);		
			fprintf(fasm, "\t\tMOV 	AL, 	BL\n");			
			fprintf(fasm, "LOGICAL_OR_END_%d:\n",ll);
		}
		fprintf(fasm, "\n");
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"U"))
	{
		if( !strcmp(root->child->next->ty->node_type,"INTEGER"))
		{
			if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"NUM"))
				fprintf(fasm, "\t\tMOV 	EAX,	%d\n", root->child->next->name.l->Value.val);

			else if(root->child->next->tag==1 && !strcmp(root->child->next->name.l->TOKEN,"ID"))
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD[RBP - %d] \n", root->child->next->symbol->offset);
			
			else if(root->child->next->tag==0 && !strcmp(root->child->next->name.nt_name,"var"))
			{
				CodeGen(root->child->next, f, st_root);
				fprintf(fasm, "\t\tMOV 	EAX,	DWORD[RDI]\n");
			}
			else if(root->child->next->tag==0)
				CodeGen(root->child->next, f, st_root);

			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"PLUS"))
				fprintf(fasm, "\t\tIMUL	EAX,	1\n");
			if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"MINUS"))
				fprintf(fasm, "\t\tIMUL	EAX,	-1\n");
		}
		return;
	}

	if(root->tag==0 && !strcmp(root->name.nt_name, "var"))
	{
		ast_node* ArrID = root->child;
		ast_node* ArrIndex = ArrID->next;
		char* Array_type = ArrID->symbol->type;
		if(!strcmp(ArrIndex->name.l->TOKEN,"NUM"))
		{
			if(ArrID->symbol->end_arr_dec!=0 && ArrID->symbol->start_arr_dec!=0)
			{
				int actual_index = ArrIndex->name.l->Value.val - ArrID->symbol->start_arr;
				int element_size = 0;
				if(!strcmp(Array_type, "BOOLEAN"))
					element_size = 1;
				else if(!strcmp(Array_type, "INTEGER"))
					element_size = 4;	
				else if(!strcmp(Array_type, "REAL"))
					element_size = 8;
				fprintf(fasm, "\t\tMOV 	RSI,	0\n");
				fprintf(fasm, "\t\tMOV 	SI, 	%d\n", ArrIndex->name.l->Value.val);
				fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);
				fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);		
				fprintf(fasm, "\t\tADD 	RDI, 	RSI\n");
				fprintf(fasm, "\n");
			}
			else
			{
				int element_size = 0;
				if(!strcmp(Array_type, "BOOLEAN"))
					element_size = 1;
				else if(!strcmp(Array_type, "INTEGER"))
					element_size = 4;	
				else if(!strcmp(Array_type, "REAL"))
					element_size = 8;
				fprintf(fasm, "\t\tMOV 	RSI,	0\n");
				fprintf(fasm, "\t\tMOV 	SI, 	%d\n", ArrIndex->name.l->Value.val);
				fprintf(fasm, "\t\tCMP 	[RBP-%d],	SI\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tJG 	SEGFAULT_ERROR\n");
				fprintf(fasm, "\t\tCMP 	[RBP-%d],	SI\n", ArrID->symbol->offset - 10);
				fprintf(fasm, "\t\tJL 	SEGFAULT_ERROR\n");
				fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
				fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);
				fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);		
				fprintf(fasm, "\t\tADD 	RDI, 	RSI\n");
				fprintf(fasm, "\n");
			}
		}
		else if(!strcmp(ArrIndex->name.l->TOKEN, "ID"))
		{
			int element_size = 0;
			if(!strcmp(Array_type, "BOOLEAN"))
				element_size = 1;
			else if(!strcmp(Array_type, "INTEGER"))
				element_size = 4;	
			else if(!strcmp(Array_type, "REAL"))
				element_size = 8;
			fprintf(fasm, "\t\tMOV 	RSI, 	0\n");
			fprintf(fasm, "\t\tMOV 	SI, 	[RBP-%d]\n", ArrIndex->symbol->offset);	
			fprintf(fasm, "\t\tCMP 	[RBP-%d],	SI\n", ArrID->symbol->offset - 8);
			fprintf(fasm, "\t\tJG 	SEGFAULT_ERROR\n");
			fprintf(fasm, "\t\tCMP 	[RBP-%d],	SI\n", ArrID->symbol->offset - 10);
			fprintf(fasm, "\t\tJL 	SEGFAULT_ERROR\n");
			fprintf(fasm, "\t\tSUB 	SI, 	[RBP-%d]\n", ArrID->symbol->offset - 8);
			fprintf(fasm, "\t\tIMUL 	SI, 	%d\n", element_size);
			fprintf(fasm, "\t\tMOV 	RDI, 	[RBP-%d]\n", ArrID->symbol->offset);		
			fprintf(fasm, "\t\tADD 	RDI, 	RSI\n");
			fprintf(fasm, "\n");
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name, "declareStmt"))
	{
		ast_node* TypeNode = root->child->next;
		if(TypeNode->tag==1);
			
		else
		{
			ast_node* arrType = TypeNode->child->next;
			ast_node* index1 = TypeNode->child->child;
			int var_size = 0;
			if(!strcmp(arrType->name.l->TOKEN, "BOOLEAN"))
				var_size = 1;
			else if(!strcmp(arrType->name.l->TOKEN, "INTEGER"))
				var_size = 4;	
			else if(!strcmp(arrType->name.l->TOKEN, "REAL"))
				var_size = 8;
			if(!strcmp(index1->name.l->TOKEN, "NUM") && !strcmp(index1->next->name.l->TOKEN, "NUM"))
			{
				if(root->child->tag==1)
				{	
					fprintf(fasm, "\t\tMOV 	RDI,	RBP\n");
					fprintf(fasm, "\t\tSUB 	RDI,	%d\n", root->child->symbol->offset - 12);
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	RDI\n", root->child->symbol->offset);
					fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", root->child->symbol->offset - 8, index1->name.l->Value.val);
					fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", root->child->symbol->offset - 10, index1->next->name.l->Value.val);
					fprintf(fasm, "\n");
				}
				else
				{
					ast_node* temp = root->child;
					while(temp->child!=NULL)
					{
						if(temp->child->next->tag==0)
						{
							fprintf(fasm, "\t\tMOV 	RDI,	RBP\n");
							fprintf(fasm, "\t\tSUB 	RDI,	%d\n", temp->child->symbol->offset - 12);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RDI\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset - 8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset - 10, index1->next->name.l->Value.val);
							fprintf(fasm, "\n");
						}
						else
						{
							fprintf(fasm, "\t\tMOV 	RDI,	RBP\n");
							fprintf(fasm, "\t\tSUB 	RDI,	%d\n", temp->child->symbol->offset - 12);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RDI\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d],  %d\n", temp->child->symbol->offset - 8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset - 10, index1->next->name.l->Value.val);
							fprintf(fasm, "\n");
							fprintf(fasm, "\t\tMOV 	RDI,	RBP\n");
							fprintf(fasm, "\t\tSUB 	RDI,	%d\n", temp->child->next->symbol->offset - 12);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RDI\n", temp->child->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->next->symbol->offset - 8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->next->symbol->offset - 10, index1->next->name.l->Value.val);
							fprintf(fasm, "\n");
						}
						temp = temp->child->next;						
					}
				}
			}
			else if(!strcmp(index1->name.l->TOKEN, "NUM") && !strcmp(index1->next->name.l->TOKEN, "ID"))
			{
				if(root->child->tag==1)
				{	
					fprintf(fasm, "\t\tMOV 	RAX,	0\n");
					fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
					fprintf(fasm, "\t\tCMP	EAX,	0\n");
					fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tCMP 	EAX,	%d\n", index1->name.l->Value.val);
					fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tSUB 	EAX,	%d\n", index1->name.l->Value.val);
					fprintf(fasm, "\t\tINC 	AX\n");
					fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
					fprintf(fasm, "\t\tMOV 	CL, 	16\n");
					fprintf(fasm, "\t\tIDIV 	CL\n");
					fprintf(fasm, "\t\tINC 	AL\n");
					fprintf(fasm, "\t\tIMUL 	CL\n");
					fprintf(fasm, "\t\tADD 	[ModuleSpace + %d], 	RAX\n", func_space);
					fprintf(fasm, "\t\tSUB 	RSP,	RAX\n");
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", root->child->symbol->offset);
					fprintf(fasm, "\t\tMOV 	WORD [RBP-%d],	%d\n", root->child->symbol->offset-8, index1->name.l->Value.val);
					fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", root->child->symbol->offset-10);
					fprintf(fasm, "\n");
				}
				else
				{
					ast_node* temp = root->child;
					while(temp->child!=NULL)
					{
						if(temp->child->next->tag==0)
						{
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP 	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB 	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tINC 	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC 	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB 	RSP, 	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV 	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset-8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX, 	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d], 	AX\n", temp->child->symbol->offset-10);
							fprintf(fasm, "\n");
						}
						else
						{
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX,	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD 	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP, 	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset-8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX, 	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d], 	AX\n", temp->child->symbol->offset-10);
							fprintf(fasm, "\n");

							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	%d\n", index1->name.l->Value.val);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX,	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD 	[ModuleSpace + %d],		RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP, 	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV	WORD [RBP-%d], %d\n", temp->child->symbol->offset-8, index1->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->symbol->offset-10);

							fprintf(fasm, "\n");
						}
						temp = temp->child->next;
					}
				}
			}
			else if(!strcmp(index1->name.l->TOKEN, "ID") && !strcmp(index1->next->name.l->TOKEN, "NUM"))
			{
				if(root->child->tag==1)
				{	
					fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tCMP	EBX,	0\n");
					fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tMOV 	RAX,	0\n");
					fprintf(fasm, "\t\tMOV	EAX,	%d\n", index1->next->name.l->Value.val);
					fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tINC	AX\n");
					fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
					fprintf(fasm, "\t\tMOV 	CL, 	16\n");
					fprintf(fasm, "\t\tIDIV 	CL\n");
					fprintf(fasm, "\t\tINC	AL\n");
					fprintf(fasm, "\t\tIMUL 	CL\n");
					fprintf(fasm, "\t\tADD 	[ModuleSpace + %d], 	RAX\n", func_space);
					fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", root->child->symbol->offset);
					fprintf(fasm, "\t\tMOV	WORD [RBP-%d], %d\n", root->child->symbol->offset-10, index1->next->name.l->Value.val);
					fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", root->child->symbol->offset-8);
					fprintf(fasm, "\n");
				}
				else
				{
					ast_node* temp = root->child;
					while(temp->child!=NULL)
					{
						if(temp->child->next->tag==0)
						{
							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");	
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	%d\n", index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD 	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV	WORD [RBP-%d], 	%d\n", temp->child->symbol->offset-10, index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->symbol->offset-8);
							fprintf(fasm, "\n");
						}
						else
						{
							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");	
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	%d\n", index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD 	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV	WORD [RBP-%d], %d\n", temp->child->symbol->offset-10, index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->symbol->offset-8);
							fprintf(fasm, "\n");

							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	%d\n", index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->next->symbol->offset);
							fprintf(fasm, "\t\tMOV	WORD [RBP-%d], %d\n", temp->child->next->symbol->offset-10, index1->next->name.l->Value.val);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->next->symbol->offset-8);
							fprintf(fasm, "\n");	
						}
						temp = temp->child->next;
					}
				}
			}
			else if(!strcmp(index1->name.l->TOKEN, "ID") && !strcmp(index1->next->name.l->TOKEN, "ID"))
			{
				if(root->child->tag==1)
				{	
					fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tCMP	EBX,	0\n");
					fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tMOV 	RAX,	0\n");
					fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
					fprintf(fasm, "\t\tCMP 	EAX,	0\n");
					fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
					fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tINC	AX\n");
					fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
					fprintf(fasm, "\t\tMOV 	CL, 	16\n");
					fprintf(fasm, "\t\tIDIV		CL\n");
					fprintf(fasm, "\t\tINC	AL\n");
					fprintf(fasm, "\t\tIMUL 	CL\n");
					fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
					fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", root->child->symbol->offset);
					fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
					fprintf(fasm, "\t\tMOV	[RBP-%d], AX\n", root->child->symbol->offset-8);
					fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
					fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", root->child->symbol->offset-10);
					fprintf(fasm, "\n");
				}
				else
				{
					ast_node* temp = root->child;
					while(temp->child!=NULL)
					{
						if(temp->child->next->tag==0)
						{
							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP 	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX,		%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV	[RBP-%d], AX\n", temp->child->symbol->offset-8);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->symbol->offset-10);
							fprintf(fasm, "\n");
						}
						else
						{
							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP 	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->symbol->offset);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV	[RBP-%d], AX\n", temp->child->symbol->offset-8);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->symbol->offset-10);
							fprintf(fasm, "\n");

							fprintf(fasm, "\t\tMOV 	EBX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tCMP	EBX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tMOV 	RAX,	0\n");
							fprintf(fasm, "\t\tMOV	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tCMP 	EAX,	0\n");
							fprintf(fasm, "\t\tJL 	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tCMP	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tJL	ARRAY_INDEX_ERROR\n");
							fprintf(fasm, "\t\tSUB	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tINC	AX\n");
							fprintf(fasm, "\t\tIMUL 	AX, 	%d\n", var_size);
							fprintf(fasm, "\t\tMOV 	CL, 	16\n");
							fprintf(fasm, "\t\tIDIV 	CL\n");
							fprintf(fasm, "\t\tINC	AL\n");
							fprintf(fasm, "\t\tIMUL 	CL\n");
							fprintf(fasm, "\t\tADD	[ModuleSpace + %d], 	RAX\n", func_space);
							fprintf(fasm, "\t\tSUB	RSP,	RAX\n");
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	RSP\n", temp->child->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->symbol->offset);
							fprintf(fasm, "\t\tMOV	[RBP-%d], AX\n", temp->child->next->symbol->offset-8);
							fprintf(fasm, "\t\tMOV 	EAX,	[RBP-%d]\n", index1->next->symbol->offset);
							fprintf(fasm, "\t\tMOV 	[RBP-%d],	AX\n", temp->child->next->symbol->offset-10);
							fprintf(fasm, "\n");
						}
						temp = temp->child->next;
					}
				}
			}
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"iterativeStmt"))
	{
		if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"FOR"))
		{
			fprintf(fasm, "\t\tMOV	DWORD [RBP-%d],	%s\n",root->child->next->symbol->offset,root->child->next->next->child->name.l->lexeme);
			for_label++;			
			int temp_label=for_label;
			fprintf(fasm, "FOR_START_%d:\n",for_label);
			fprintf(fasm, "\t\tMOV 	ECX,	DWORD [RBP-%d]\n",root->child->next->symbol->offset);
			fprintf(fasm, "\t\tCMP	ECX, 	%s\n",root->child->next->next->child->next->name.l->lexeme);
			fprintf(fasm, "\t\tJG 	FOR_END_%d\n",for_label);
			fprintf(fasm, "\n");
			CodeGen(root->child->next->next->next->next, f, st_root);
			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tADD 	DWORD [RBP-%d], 	1\n",root->child->next->symbol->offset);
			fprintf(fasm, "\t\tJMP	FOR_START_%d\n",temp_label);
			fprintf(fasm, "FOR_END_%d:\n",temp_label);
			fprintf(fasm, "\n");
			return;
		}
		if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"WHILE"))
		{
			while_label++;
			int temp_label=while_label;
			fprintf(fasm, "WHILE_START_%d:\n",while_label);
			fprintf(fasm, "\n");		
			CodeGen(root->child->next, f, st_root); 
			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tCMP 	AL, 	0\n");
			fprintf(fasm, "\t\tJE 	WHILE_END_%d\n",while_label);
			CodeGen(root->child->next->next->next, f, st_root);
			fprintf(fasm, "\t\tJMP 	WHILE_START_%d\n",temp_label);
			fprintf(fasm, "WHILE_END_%d:\n",temp_label);
			return ;	
		}
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"conditionalStmt"))
	{
		switch_label++;
		f.switch_label=switch_label;
		f.cond_offset=root->child->symbol->offset;
		CodeGen(root->child->next->next,f, st_root);	
		fprintf(fasm, "SWITCH_DEFAULT_%d:\n\n",f.switch_label);
		if(root->child->next->next->next!=NULL && root->child->next->next->next->tag==0 && !strcmp(root->child->next->next->next->name.nt_name,"default"))
			CodeGen(root->child->next->next->next,f, st_root);				
		fprintf(fasm, "\n");
		fprintf(fasm, "SWITCH_END_%d:\n",f.switch_label);
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name,"caseStmts") || root->tag==0 && !strcmp(root->name.nt_name,"caseStmt"))
	{
		if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"NUM"))
		{
			fprintf(fasm, "\t\tCMP	DWORD [RBP-%d],	%s\n",f.cond_offset,root->child->name.l->lexeme);			
			fprintf(fasm, "\t\tJE 	CASE_%d_%s\n",f.switch_label,root->child->name.l->lexeme);
			if(root->child->next==NULL)
				fprintf(fasm, "\t\tJMP	SWITCH_DEFAULT_%d\n",f.switch_label);

			else if(root->child->next!=NULL && !strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next,f, st_root);

			else if(root->child->next->next!=NULL && !strcmp(root->child->next->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next->next,f, st_root);	

			else
				fprintf(fasm, "\t\tJMP	SWITCH_DEFAULT_%d\n",f.switch_label);		

			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tCASE_%d_%s:\n",f.switch_label,root->child->name.l->lexeme);
			
			if(root->child->next==NULL);
			
			else if(root->child->next!=NULL && strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next, f, st_root);
			
			fprintf(fasm, "\t\tJMP	SWITCH_END_%d\n",f.switch_label);
			fprintf(fasm, "\n");
			return;
		}
		else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"TRUE"))
		{
			fprintf(fasm, "\t\tCMP	BYTE [RBP-%d],	1\n",f.cond_offset);			
			fprintf(fasm, "\t\tJE 	CASE_%d_TRUE\n", f.switch_label);
			
			if(root->child->next==NULL);	
			
			else if(root->child->next!=NULL && !strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next, f, st_root);
			
			else if(root->child->next->next!=NULL && !strcmp(root->child->next->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next->next,f, st_root);
			
			else
				fprintf(fasm, "\t\tJMP 	SWITCH_DEFAULT_%d \n",f.switch_label);		
			
			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tCASE_%d_TRUE:\n",f.switch_label);
			
			if(root->child->next==NULL);
			
			if(root->child->next!=NULL && strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next, f, st_root);
			
			fprintf(fasm, "\t\tJMP	SWITCH_END_%d\n",f.switch_label);
			fprintf(fasm, "\n");
			return;
		}	
		else if(root->child->tag==1 && !strcmp(root->child->name.l->TOKEN,"FALSE"))
		{
			fprintf(fasm, "\t\tCMP	BYTE [RBP-%d],	0\n",f.cond_offset);			
			fprintf(fasm, "\t\tJE 	CASE_%d_FALSE\n", f.switch_label);
			
			if(root->child->next==NULL);	
			
			else if(root->child->next!=NULL && !strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next, f, st_root);
			
			else if(root->child->next->next!=NULL && !strcmp(root->child->next->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next->next, f, st_root);			
			
			else
				fprintf(fasm, "\t\tJMP 	SWITCH_DEFAULT_%d \n",f.switch_label);			
			
			fprintf(fasm, "\n");
			fprintf(fasm, "\t\tCASE_%d_FALSE:\n",f.switch_label);
			
			if(root->child->next==NULL);
			
			if(root->child->next!=NULL && strcmp(root->child->next->name.nt_name,"caseStmt"))
				CodeGen(root->child->next, f, st_root);
			
			fprintf(fasm, "\t\tJMP	SWITCH_END_%d\n",f.switch_label);		
			fprintf(fasm, "\n");
		}
		return;	
	}
	if(root->tag==0 && !strcmp(root->name.nt_name, "ioStmt"))
	{
		if(!strcmp(root->child->name.l->TOKEN, "PRINT"))
		{
			fprintf(fasm, "\t\tMOV 		EAX,		4\n");
			fprintf(fasm, "\t\tMOV 		EBX,		1\n");
			fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
			fprintf(fasm, "\t\tMOV 		EDX,		3\n");
			fprintf(fasm, "\t\tINT 		80H\n");
			fprintf(fasm, "\t\tMOV 		EAX,		4\n");
			fprintf(fasm, "\t\tMOV 		EBX,		1\n");
			fprintf(fasm, "\t\tMOV 		ECX,		output\n");
			fprintf(fasm, "\t\tMOV 		EDX,		outlen\n");
			fprintf(fasm, "\t\tINT 		80H	\n");
			fprintf(fasm, "\n");
			if(root->child->next->tag==1)
			{
				ast_node* id_lex = root->child->next;
				if(strcmp(id_lex->name.l->TOKEN, "ID"))
				{
					if(!strcmp(id_lex->name.l->TOKEN, "TRUE"))
					{
						fprintf(fasm, "\t\tMOV 		EAX,		4\n");
						fprintf(fasm, "\t\tMOV 		EBX,		1\n");
						fprintf(fasm, "\t\tMOV 		ECX,		true\n");
						fprintf(fasm, "\t\tMOV 		EDX,		trulen\n");
						fprintf(fasm, "\t\tINT 		80H \n");
						fprintf(fasm, "\n");
					  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
						fprintf(fasm, "\t\tMOV 	EBX,	1\n");
						fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
						fprintf(fasm, "\t\tMOV 	EDX,	2\n");
						fprintf(fasm, "\t\tINT 	80H\n");
						fprintf(fasm, "\n");
					}
						else if(!strcmp(id_lex->name.l->TOKEN, "FALSE"))
					{
						fprintf(fasm, "\t\tMOV 		EAX,		4\n");
						fprintf(fasm, "\t\tMOV 		EBX,		1\n");
						fprintf(fasm, "\t\tMOV 		ECX,		false\n");
						fprintf(fasm, "\t\tMOV 		EDX,		fallen\n");
						fprintf(fasm, "\t\tINT 		80H\n");
						fprintf(fasm, "\n");
					  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
						fprintf(fasm, "\t\tMOV 	EBX,	1\n");
						fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
						fprintf(fasm, "\t\tMOV 	EDX,	2\n");
						fprintf(fasm, "\t\tINT 	80H\n");
						fprintf(fasm, "\n");
					}
					else if(!strcmp(id_lex->name.l->TOKEN, "NUM"))
					{
						fprintf(fasm, "\t\tMOV 		EAX,		%d\n", id_lex->name.l->Value.val);
						fprintf(fasm, "\t\tCALL		printEAX\n");
						fprintf(fasm, "\n");
					  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
						fprintf(fasm, "\t\tMOV 	EBX,	1\n");
						fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
						fprintf(fasm, "\t\tMOV 	EDX,	2\n");
						fprintf(fasm, "\t\tINT 	80H\n");
						fprintf(fasm, "\n");
					}
					else if(!strcmp(id_lex->name.l->TOKEN, "RNUM"))
					{
						floatnum++;
						fprintf(fasm, "\t\t\t\t\t\t\tsection		.data\n");
						fprintf(fasm, "FLOAT%d 	DQ 		%lf\n", floatnum ,id_lex->name.l->Value.real_val);
						fprintf(fasm, "\t\t\t\t\t\t\tsection		.text\n\n");
						fprintf(fasm, "\t\tMOVQ 	XMM0,	QWORD [FLOAT%d]\n", floatnum);
						fprintf(fasm, "\t\tMOV 	RDI,	FloatPrint\n");
						fprintf(fasm, "\t\tMOV 	RAX,	1\n");
						fprintf(fasm, "\t\tCALL 	printf\n");
					}
					fprintf(fasm, "\n");
				}
				else
				{
					if(id_lex->symbol->arr==0)
					{
						if(!strcmp(id_lex->symbol->type, "INTEGER"))
						{
							fprintf(fasm, "\t\tMOV 		EAX,		[RBP-%d]\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tCALL		printEAX\n");
							fprintf(fasm, "\n");
						  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
							fprintf(fasm, "\t\tMOV 	EBX,	1\n");
							fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
							fprintf(fasm, "\t\tMOV 	EDX,	2\n");
							fprintf(fasm, "\t\tINT 	80H\n");
							fprintf(fasm, "\n");
						}
						else if(!strcmp(id_lex->symbol->type, "BOOLEAN"))
						{
							boolprint++;
							fprintf(fasm, "\t\tCMP		BYTE [RBP-%d],		0\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tJNZ		TRUE_%d\n",		boolprint);
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		false\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		fallen\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
							fprintf(fasm, "\t\t\tJMP		BOOLPRINT_%d\n", boolprint);
						   	fprintf(fasm, "TRUE_%d:\n", boolprint);
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		true\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		trulen\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
						   	fprintf(fasm, "BOOLPRINT_%d:\n", boolprint);
						   	fprintf(fasm, "\n");
						  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
							fprintf(fasm, "\t\tMOV 	EBX,	1\n");
							fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
							fprintf(fasm, "\t\tMOV 	EDX,	2\n");
							fprintf(fasm, "\t\tINT 	80H\n");
							fprintf(fasm, "\n");
						}
						else if(!strcmp(id_lex->symbol->type, "REAL"))
						{
							fprintf(fasm, "\t\tMOVQ	XMM0,	[RBP-%d]\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tLEA 	RDI,	[FloatPrint]\n");
							fprintf(fasm, "\t\tMOV 	RAX,	1\n");
							fprintf(fasm, "\t\tCALL	printf\n");
						}
						fprintf(fasm, "\n");
					}
					else 
					{
						if(!strcmp(id_lex->symbol->type, "INTEGER"))
						{
							intarrprint++;
							fprintf(fasm, "\t\tMOV		RDI,		[RBP-%d]\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tMOV 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 10);
							fprintf(fasm, "\t\tSUB 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 8);
							fprintf(fasm, "\t\tINC		DX\n");
							fprintf(fasm, "\t\tMOV 		BX, 		0\n");
						   	fprintf(fasm, "INT_ARR_PRINT_%d:\n", intarrprint);	
							fprintf(fasm, "\t\t\tMOV 		RSI,		0\n");
							fprintf(fasm, "\t\t\tMOV 		SI,		BX\n");
							fprintf(fasm, "\t\t\tMOV 		EAX,		[RDI+RSI*4]\n");
							fprintf(fasm, "\t\t\tCALL		printEAX\n");
							fprintf(fasm, "\t\t\tPUSH		RBX\n");
							fprintf(fasm, "\t\t\tPUSH		RDX\n");
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		Space\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		1\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
							fprintf(fasm, "\t\t\tPOP		RDX\n");
							fprintf(fasm, "\t\t\tPOP		RBX\n");
							fprintf(fasm, "\t\t\tINC 		BX\n");
							fprintf(fasm, "\t\t\tCMP		BX,		DX	\n");					
							fprintf(fasm, "\t\tJNE		INT_ARR_PRINT_%d\n", intarrprint);
							fprintf(fasm, "\n");
						  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
							fprintf(fasm, "\t\tMOV 	EBX,	1\n");
							fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
							fprintf(fasm, "\t\tMOV 	EDX,	2\n");
							fprintf(fasm, "\t\tINT 	80H\n");
							fprintf(fasm, "\n");
						}
						else if(!strcmp(id_lex->symbol->type, "BOOLEAN"))
						{
							boolarrprint++;
							fprintf(fasm, "\t\tMOV		RDI,		[RBP-%d]\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tMOV 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 10);
							fprintf(fasm, "\t\tSUB 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 8);
							fprintf(fasm, "\t\tINC		DX\n");
							fprintf(fasm, "\t\tMOV 		[NumEle],	DX\n");
							fprintf(fasm, "\t\tMOV 		RSI,		0\n");
						  	fprintf(fasm, "BOOL_ARR_PRINT_%d:\n", boolarrprint);
							fprintf(fasm, "\t\t\tCMP		BYTE [RDI+RSI],		0\n");
							fprintf(fasm, "\t\t\tJNZ		ARRAY_TRUE_%d\n", boolarrprint);
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		false\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		fallen\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
					  		fprintf(fasm, "\t\t\tJMP		BOOL_SPACE_%d\n", boolarrprint);	
						  	fprintf(fasm, "ARRAY_TRUE_%d:\n", boolarrprint);					
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		true\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		trulen\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
						 	fprintf(fasm, "BOOL_SPACE_%d:\n", boolarrprint);
							fprintf(fasm, "\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\tMOV 		ECX,		Space\n");
							fprintf(fasm, "\t\tMOV 		EDX,		1\n");
							fprintf(fasm, "\t\tINT 		80H\n");
							fprintf(fasm, "\t\tINC		RSI\n");
							fprintf(fasm, "\t\tCMP		SI,		[NumEle]\n");
							fprintf(fasm, "\t\tJNE		BOOL_ARR_PRINT_%d\n", boolarrprint);
							fprintf(fasm, "\n");
						  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
							fprintf(fasm, "\t\tMOV 	EBX,	1\n");
							fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
							fprintf(fasm, "\t\tMOV 	EDX,	2\n");
							fprintf(fasm, "\t\tINT 	80H\n");
							fprintf(fasm, "\n");
						}
						else if(!strcmp(id_lex->symbol->type, "REAL"))
						{
							realarrprint++;
							fprintf(fasm, "\t\tMOV		RDI,		[RBP-%d]\n", id_lex->symbol->offset);
							fprintf(fasm, "\t\tMOV 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 10);
							fprintf(fasm, "\t\tSUB 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 8);
							fprintf(fasm, "\t\tINC		DX\n");
							fprintf(fasm, "\t\tMOV 		BX, 		0\n");
						   	fprintf(fasm, "REAL_ARR_PRINT_%d:\n", realarrprint);	
							fprintf(fasm, "\t\t\tMOV 		RSI,		0\n");
							fprintf(fasm, "\t\t\tMOV 		SI,		BX\n");
							fprintf(fasm, "\t\t\tPUSH		RDI\n");
							fprintf(fasm, "\t\t\tPUSH		RSI\n");
							fprintf(fasm, "\t\t\tPUSH		RDI\n");
							fprintf(fasm, "\t\t\tPUSH		RSI\n");	
							fprintf(fasm, "\t\t\tMOVQ 		XMM0,		[RDI+RSI*8]\n");
							fprintf(fasm, "\t\t\tMOV 	RDI,	FloatPrint\n");
							fprintf(fasm, "\t\t\tMOV 	RAX,	1\n");
							fprintf(fasm, "\t\t\tCALL 	printf\n");
							fprintf(fasm, "\t\t\tPOP		RSI\n");
							fprintf(fasm, "\t\t\tPOP		RDI\n");
							fprintf(fasm, "\t\t\tPOP		RSI\n");
							fprintf(fasm, "\t\t\tPOP		RDI\n");
							fprintf(fasm, "\t\t\tPUSH		RBX\n");
							fprintf(fasm, "\t\t\tPUSH		RDX\n");
							fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
							fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
							fprintf(fasm, "\t\t\tMOV 		ECX,		Space\n");
							fprintf(fasm, "\t\t\tMOV 		EDX,		1\n");
							fprintf(fasm, "\t\t\tINT 		80H\n");
							fprintf(fasm, "\t\t\tPOP		RDX\n");
							fprintf(fasm, "\t\t\tPOP		RBX\n");
							fprintf(fasm, "\t\t\tINC 		BX\n");
							fprintf(fasm, "\t\t\tMOV 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 10);
							fprintf(fasm, "\t\t\tSUB 		DX,		[RBP-%d]\n", id_lex->symbol->offset - 8);
							fprintf(fasm, "\t\t\tINC		DX\n");
							fprintf(fasm, "\t\t\tCMP		BX,		DX	\n");					
							fprintf(fasm, "\t\tJNE		REAL_ARR_PRINT_%d\n", realarrprint);
							fprintf(fasm, "\n");
						}
						fprintf(fasm, "\n");
					}
				}
			}
			else
			{
				CodeGen(root->child->next, f, st_root);
				if(!strcmp(root->child->next->child->symbol->type, "INTEGER"))
				{
					fprintf(fasm, "\t\tMOV		EAX,		[RDI]\n");
					fprintf(fasm, "\t\tCALL 	printEAX\n");
					fprintf(fasm, "\n");
				  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
					fprintf(fasm, "\t\tMOV 	EBX,	1\n");
					fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
					fprintf(fasm, "\t\tMOV 	EDX,	2\n");
					fprintf(fasm, "\t\tINT 	80H\n");
					fprintf(fasm, "\n");
				}
				else  if(!strcmp(root->child->next->child->symbol->type, "BOOLEAN"))
				{
					boolelemprint++;
					fprintf(fasm, "\t\tMOV 		AL,		[RDI]\n");
					fprintf(fasm, "\t\tCMP		AL,		0\n");
					fprintf(fasm, "\t\tJNZ		TRUE_ELEM_%d\n", boolelemprint);
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		false\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		fallen\n");
					fprintf(fasm, "\t\t\tINT		80H\n");
				  	fprintf(fasm, "\t\t\tJMP		BOOL_ELEM_PRINT_%d\n", boolelemprint);
				  	fprintf(fasm, "TRUE_ELEM_%d:\n", boolelemprint);
			 		fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		true\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		trulen\n");
					fprintf(fasm, "\t\t\tINT			80H\n");
				  	fprintf(fasm, "BOOL_ELEM_PRINT_%d:\n",	boolelemprint);
				  	fprintf(fasm, "\n");
				  	fprintf(fasm, "\t\tMOV 	EAX,	4\n");
					fprintf(fasm, "\t\tMOV 	EBX,	1\n");
					fprintf(fasm, "\t\tMOV 	ECX, 	NewLine\n");
					fprintf(fasm, "\t\tMOV 	EDX,	2\n");
					fprintf(fasm, "\t\tINT 	80H\n");
					fprintf(fasm, "\n");
				}
				else if(!strcmp(root->child->next->child->symbol->type, "REAL"))
				{
					fprintf(fasm, "\t\tMOVQ	XMM0,	QWORD [RDI]\n");
					fprintf(fasm, "\t\tMOV 	RDI,	FloatPrint\n");
					fprintf(fasm, "\t\tMOV 	RAX,	1\n");
					fprintf(fasm, "\t\tCALL	printf\n");
				}
				fprintf(fasm, "\n");
			}
			return;
		}

		else if(!strcmp(root->child->name.l->TOKEN, "GET_VALUE"))
		{
			ast_node* var = root->child->next;
			if(var->symbol->arr==0)
			{
				if(!strcmp(var->symbol->type, "INTEGER"))
				{
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		an\n");
					fprintf(fasm, "\t\tMOV 		EDX,		anlen\n");
					fprintf(fasm, "\t\tINT 		80H	\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		integer\n");
					fprintf(fasm, "\t\tMOV 		EDX,		intlen\n");
					fprintf(fasm, "\t\tINT 		80H	\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		value\n");
					fprintf(fasm, "\t\tMOV 		EDX,		vallen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\tMOV 		EDX,		termlen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EDI,		IntFmt\n");
					fprintf(fasm, "\t\tMOV 		ESI,		InpBuffer\n");
					fprintf(fasm, "\t\tCALL		scanf\n");

					fprintf(fasm, "\t\tMOV 		EAX,		[InpBuffer]\n");
					fprintf(fasm, "\t\tCALL		CLR_IPBUFFER\n");
					fprintf(fasm, "\t\tMOV 		[RBP-%d],	EAX\n",	var->symbol->offset);

				}
				else if(!strcmp(var->symbol->type, "BOOLEAN"))
				{
					getbool++;
					fprintf(fasm, "BOOL_GET_%d:\n", getbool);
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		a\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		alen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		boolean\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		boollen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		value\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		vallen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		termlen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					
					fprintf(fasm, "\t\t\tMOV 		EDI,		IntFmt\n");
					fprintf(fasm, "\t\t\tMOV 		ESI,		InpBuffer\n");
					fprintf(fasm, "\t\t\tCALL		scanf\n");
				
					fprintf(fasm, "\t\t\tMOV 		AL,		[InpBuffer]\n");
					fprintf(fasm, "\t\t\tCALL		CLR_IPBUFFER\n");
					fprintf(fasm, "\t\t\tCMP		AL,		1\n");
					fprintf(fasm, "\t\t\tJE		GOT_BOOL_%d\n", getbool);
					fprintf(fasm, "\t\t\tCMP		AL,		0\n");
					fprintf(fasm, "\t\t\tJE		GOT_BOOL_%d\n",	getbool);
					fprintf(fasm, "\t\tMOV		EAX,	4\n");
					fprintf(fasm, "\t\tMOV		EBX,	1\n");
					fprintf(fasm, "\t\tMOV		ECX, 	Input_Error\n");
					fprintf(fasm, "\t\tMOV		EDX,	IPErr\n");
					fprintf(fasm, "\t\tINT		80H\n");
					fprintf(fasm, "\t\tJMP		BOOL_GET_%d\n", getbool);
				  	fprintf(fasm, "GOT_BOOL_%d:\n", getbool);
					fprintf(fasm, "\t\tMOV 		[RBP-%d],	AL\n",	var->symbol->offset);
				}
				else if(!strcmp(var->symbol->type, "REAL"))
				{
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		a\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		alen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		real\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		realen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		value\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		vallen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		termlen\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 	EDI,	FloatScan\n");
					fprintf(fasm, "\t\tMOV 	RSI,	FloatBuff\n");
					fprintf(fasm, "\t\tXOR 	RAX,	RAX\n");
					fprintf(fasm, "\t\tCALL 	scanf\n");

					fprintf(fasm, "\t\tMOVQ 	XMM0,	[FloatBuff]\n");
					fprintf(fasm, "\t\tMOVQ 	[RBP-%d],	XMM0\n", var->symbol->offset);
				}
				fprintf(fasm, "\n");
			}
			else
			{
				if(!strcmp(var->symbol->type, "INTEGER"))
				{
					getintarr++;
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		RAX,		0\n");
					fprintf(fasm, "\t\tMOV 		BX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tMOV		CX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tSUB		BX,		CX\n");
					fprintf(fasm, "\t\tMOV 		AX,		BX\n");
					fprintf(fasm, "\t\tINC		AX\n");
					fprintf(fasm, "\t\tMOV 		[NumEle],	EAX\n");
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		array\n");
					fprintf(fasm, "\t\tMOV 		EDX,		arrlen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		integer\n");
					fprintf(fasm, "\t\tMOV 		EDX,		intlen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		type\n");
					fprintf(fasm, "\t\tMOV 		EDX,		typelen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		to_\n");
					fprintf(fasm, "\t\tMOV 		EDX,		tolen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		NewLine\n");
					fprintf(fasm, "\t\tMOV 		EDX,		2\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		RSI,		0\n");
					fprintf(fasm, "\t\tMOV 		RDI,		[RBP-%d]\n", var->symbol->offset);
				  	fprintf(fasm, "GET_INT_ARR_%d:\n",	getintarr);
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					
					fprintf(fasm, "\t\t\tPUSH		RDI\n");
					fprintf(fasm, "\t\t\tPUSH		RSI\n");
					fprintf(fasm, "\t\t\tMOV 		EDI,		IntFmt\n");
					fprintf(fasm, "\t\t\tMOV 		ESI,		InpBuffer\n");
					fprintf(fasm, "\t\t\tCALL		scanf\n");
					fprintf(fasm, "\t\t\tPOP		RSI\n");
					fprintf(fasm, "\t\t\tPOP		RDI\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		[InpBuffer]\n");
					fprintf(fasm, "\t\t\tCALL		CLR_IPBUFFER\n");
					
					fprintf(fasm, "\t\t\tMOV 		[RDI + RSI*4],	EAX\n");
					fprintf(fasm, "\t\t\tINC		SI\n");
					fprintf(fasm, "\t\t\tCMP		ESI,		[NumEle]\n");
					fprintf(fasm, "\t\tJNE		GET_INT_ARR_%d\n", getintarr);
				}
				else if(!strcmp(var->symbol->type, "BOOLEAN"))
				{
					getboolarr++;
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV 		BX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tMOV		CX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tSUB		BX,		CX\n");
					fprintf(fasm, "\t\tMOV 		AX,		BX\n");
					fprintf(fasm, "\t\tINC		AX\n");
					fprintf(fasm, "\t\tMOV 		[NumEle],	EAX\n");
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		array\n");
					fprintf(fasm, "\t\tMOV 		EDX,		arrlen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		boolean\n");
					fprintf(fasm, "\t\tMOV 		EDX,		boollen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		type\n");
					fprintf(fasm, "\t\tMOV 		EDX,		typelen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		to_\n");
					fprintf(fasm, "\t\tMOV 		EDX,		tolen\n");
					fprintf(fasm, "\t\tINT 		80H\n");	
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		NewLine\n");
					fprintf(fasm, "\t\tMOV 		EDX,		2\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		RSI,		0\n");
					fprintf(fasm, "\t\tMOV 		RDI,		[RBP-%d]\n", var->symbol->offset);
				  	fprintf(fasm, "GET_BOOL_ARR_%d:\n", getboolarr);
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					
					fprintf(fasm, "\t\t\tPUSH		RDI\n");
					fprintf(fasm, "\t\t\tPUSH		RSI\n");
					fprintf(fasm, "\t\t\tMOV 		EDI,		IntFmt\n");
					fprintf(fasm, "\t\t\tMOV 		ESI,		InpBuffer\n");
					fprintf(fasm, "\t\t\tCALL		scanf\n");
					fprintf(fasm, "\t\t\tPOP		RSI\n");
					fprintf(fasm, "\t\t\tPOP		RDI\n");
					fprintf(fasm, "\t\t\tMOV 		EAX,		[InpBuffer]\n");
					fprintf(fasm, "\t\t\tCALL		CLR_IPBUFFER\n");
					fprintf(fasm, "\t\t\tCMP		AL,		1\n");
					fprintf(fasm, "\t\t\tJE		GET_BOOL_CONT_%d\n", getboolarr);
					fprintf(fasm, "\t\t\tCMP		AL,		0\n");
					fprintf(fasm, "\t\t\tJE		GET_BOOL_CONT_%d\n", getboolarr);
					fprintf(fasm, "\t\tMOV		EAX,	4\n");
					fprintf(fasm, "\t\tMOV		EBX,	1\n");
					fprintf(fasm, "\t\tMOV		ECX, 	Input_Error\n");
					fprintf(fasm, "\t\tMOV		EDX,	IPErr\n");
					fprintf(fasm, "\t\tINT		80H\n");
					fprintf(fasm, "\t\tJMP		GET_BOOL_ARR_%d\n", getboolarr);	
				  	fprintf(fasm, "GET_BOOL_CONT_%d:\n", getboolarr);
					fprintf(fasm, "\t\tMOV 		[RDI + RSI],	EAX\n");
					fprintf(fasm, "\t\tINC		SI\n");
					fprintf(fasm, "\t\tCMP		ESI,		[NumEle]\n");
					fprintf(fasm, "\t\tJNE		GET_BOOL_ARR_%d\n", getboolarr);
				}
				else if(!strcmp(var->symbol->type, "REAL"))
				{
					getrealarr++;
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		input\n");
					fprintf(fasm, "\t\tMOV 		EDX,		iplen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		RAX,		0\n");
					fprintf(fasm, "\t\tMOV 		BX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tMOV		CX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tSUB		BX,		CX\n");
					fprintf(fasm, "\t\tMOV 		AX,		BX\n");
					fprintf(fasm, "\t\tINC		AX\n");
					fprintf(fasm, "\t\tMOV 		[NumEle],	EAX\n");
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		array\n");
					fprintf(fasm, "\t\tMOV 		EDX,		arrlen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		real\n");
					fprintf(fasm, "\t\tMOV 		EDX,		realen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		type\n");
					fprintf(fasm, "\t\tMOV 		EDX,		typelen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-8);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		to_\n");
					fprintf(fasm, "\t\tMOV 		EDX,		tolen\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		EAX,		0\n");
					fprintf(fasm, "\t\tMOV		AX,		[RBP-%d]\n", var->symbol->offset-10);
					fprintf(fasm, "\t\tCALL		printEAX\n");
					fprintf(fasm, "\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\tMOV 		ECX,		NewLine\n");
					fprintf(fasm, "\t\tMOV 		EDX,		2\n");
					fprintf(fasm, "\t\tINT 		80H\n");
					fprintf(fasm, "\t\tMOV 		RSI,		0\n");
					fprintf(fasm, "\t\tMOV 		RDI,		[RBP-%d]\n", var->symbol->offset);
				  	fprintf(fasm, "GET_REAL_ARR_%d:\n",	getrealarr);
					fprintf(fasm, "\t\t\tMOV 		EAX,		4\n");
					fprintf(fasm, "\t\t\tMOV 		EBX,		1\n");
					fprintf(fasm, "\t\t\tMOV 		ECX,		terminal\n");
					fprintf(fasm, "\t\t\tMOV 		EDX,		3\n");
					fprintf(fasm, "\t\t\tINT 		80H\n");
					
					fprintf(fasm, "\t\t\tPUSH		RDI\n");
					fprintf(fasm, "\t\t\tPUSH		RSI\n");
					fprintf(fasm, "\t\t\tMOV 		EDI,		FloatScan\n");
					fprintf(fasm, "\t\t\tMOV 		RSI,		FloatBuff\n");
					fprintf(fasm, "\t\t\tCALL		scanf\n");
					fprintf(fasm, "\t\t\tPOP		RSI\n");
					fprintf(fasm, "\t\t\tPOP		RDI\n");
					fprintf(fasm, "\t\t\tMOVQ 		XMM0,		[FloatBuff]\n");
					
					fprintf(fasm, "\t\t\tMOVQ 		[RDI + RSI*8],	XMM0\n");
					fprintf(fasm, "\t\t\tINC		SI\n");
					fprintf(fasm, "\t\t\tCMP		ESI,		[NumEle]\n");
					fprintf(fasm, "\t\tJNE		GET_REAL_ARR_%d\n", getrealarr);
				}
				fprintf(fasm, "\n");

			}
		}
		return;
	}
	if(root->tag==0 && !strcmp(root->name.nt_name, "moduleReuseStmt"))
	{
		func_space+=8;		
		st_node* func;
		if(root->child->tag==1)		
			func = lookup_function_main_scope_table(root->child->name.l->lexeme, st_root, 1);
		else if(root->child->next->tag==1)		
			func = lookup_function_main_scope_table(root->child->next->name.l->lexeme, st_root, 1);

		fprintf(fasm, "\t\tMOV 	RSI,	RBP\n");
		fprintf(fasm, "\t\tPUSH 	RSI\n");
		fprintf(fasm, "\t\tPUSH 	RSI\n");
		fprintf(fasm, "\t\tMOV 	RBP,	RSP\n");
		fprintf(fasm, "\n");
		if(func->maxoffset%16==0)
		{
			fprintf(fasm, "\t\tMOV 	QWORD [ModuleSpace + %d],	%d\n", func_space, func->maxoffset);			
			fprintf(fasm, "\t\tSUB	RSP,	%d\n", func->maxoffset);
		}
		else
		{
				fprintf(fasm, "\t\tMOV 	QWORD [ModuleSpace + %d],	%d\n", func_space, (1+func->maxoffset/16)*16);
				fprintf(fasm, "\t\tSUB	RSP,	%d\n", (1+func->maxoffset/16)*16);
		}		
		ast_node* insert_list;
		ast_node* current=root->child;
		while(current->next!=NULL)
			current=current->next;
		insert_list=current;

		intialise_inp_vars(func->module_ptr->child->next, insert_list);
		ast_node* moddef;		
		current=func->module_ptr->child;
		while(current->next!=NULL)
			current=current->next;
		moddef=current;
		fprintf(fasm, "\n");
		CodeGen(moddef, f, st_root);
		fprintf(fasm, "\n");	
		fprintf(fasm, "\t\tMOV	RAX,	QWORD [ModuleSpace + %d]\n", func_space);
		fprintf(fasm, "\t\tMOV	QWORD[ModuleSpace + %d],	0\n", func_space);
		func_space-=8;
		fprintf(fasm, "\n");
		fprintf(fasm, "\t\tADD	RSP,	RAX\n");	
		fprintf(fasm, "\t\tPOP	RSI\n");
		fprintf(fasm, "\t\tPOP	RSI\n");
		current=func->module_ptr->child;
		while(current->next!=NULL)
		{
			if(current->tag==0 && (!strcmp(current->name.nt_name,"output_plist")))
			{
				fill_ret_vars(current, root->child->child);
				break;			
			}			
			current=current->next;
		}
		fprintf(fasm, "\t\tMOV 	RBP,	RSI\n");
		fprintf(fasm, "\n");
		return;
	}	
	ast_node* child=root->child;
	while(child!=NULL)
	{
		CodeGen(child,f, st_root);
		child=child->next;	
	}
	return;
}



void fill_ret_vars(ast_node* input_list, ast_node* insert_list)
{
	if(input_list==NULL || insert_list==NULL)
		return;

	if((input_list->child->tag==1 && !strcmp(input_list->child->name.l->TOKEN,"ID")) &&  (insert_list->tag==1 && !strcmp(insert_list->name.l->TOKEN,"ID")))
	{
		if(input_list->child->symbol->arr==1 && insert_list->symbol->arr==1)
		{
			fprintf(fasm, "\t\tMOV	RAX,	QWORD [RBP-%d]\n", input_list->child->symbol->offset);
			fprintf(fasm, "\t\tMOV	QWORD [RSI-%d],	RAX\n", insert_list->symbol->offset);
			fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", input_list->child->symbol->offset - 8);
			fprintf(fasm, "\t\tMOV	WORD [RSI-%d],	AX\n", insert_list->symbol->offset - 8);
			fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", input_list->child->symbol->offset - 10);
			fprintf(fasm, "\t\tMOV	WORD [RSI-%d],	AX\n", insert_list->symbol->offset - 10);	
		}
		else if(!strcmp(input_list->child->symbol->type, "INTEGER") && !strcmp(insert_list->symbol->type, "INTEGER"))
		{
			fprintf(fasm, "\t\tMOV 	EAX, 	DWORD [RBP-%d]\n", input_list->child->symbol->offset);
			fprintf(fasm, "\t\tMOV	DWORD [RSI-%d],	EAX\n", insert_list->symbol->offset);
		}
		else if(!strcmp(input_list->child->symbol->type, "BOOLEAN") && !strcmp(insert_list->symbol->type, "BOOLEAN"))
		{
			fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RBP-%d]\n", input_list->child->symbol->offset);
			fprintf(fasm, "\t\tMOV	BYTE [RSI-%d],	AL\n", insert_list->symbol->offset);
		}
		else if(!strcmp(input_list->child->symbol->type, "REAL") && !strcmp(insert_list->symbol->type, "REAL"))
		{
			fprintf(fasm, "\t\tMOV	RAX,	QWORD [RBP-%d]\n", input_list->child->symbol->offset);
			fprintf(fasm, "\t\tMOV	QWORD [RSI-%d],	RAX\n", insert_list->symbol->offset);
		}
		fprintf(fasm, "\n");
		return;
	}
	else if(input_list->tag==0 &&  insert_list->tag==0 )
	{
		if((input_list->child->tag==1 && !strcmp(input_list->child->name.l->TOKEN,"ID")) &&  (insert_list->child->tag==1 && !strcmp(insert_list->child->name.l->TOKEN,"ID")))
		{
			if(input_list->child->symbol->arr==1 && insert_list->child->symbol->arr==1)
			{
				fprintf(fasm, "\t\tMOV	RAX,	QWORD [RBP-%d]\n", input_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	QWORD [RSI-%d],	RAX\n", insert_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", input_list->child->symbol->offset - 8);
				fprintf(fasm, "\t\tMOV	WORD [RSI-%d],	AX\n", insert_list->child->symbol->offset - 8);
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RBP-%d]\n", input_list->child->symbol->offset - 10);
				fprintf(fasm, "\t\tMOV	WORD [RSI-%d],	AX\n", insert_list->child->symbol->offset- 10);
			}
			else if(!strcmp(input_list->child->symbol->type, "INTEGER") && !strcmp(insert_list->child->symbol->type, "INTEGER"))
			{
				fprintf(fasm, "\t\tMOV 	EAX, 	DWORD [RBP-%d]\n", input_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	DWORD [RSI-%d],	EAX\n", insert_list->child->symbol->offset);
			}
			else if(!strcmp(input_list->child->symbol->type, "BOOLEAN") && !strcmp(insert_list->child->symbol->type, "BOOLEAN"))
			{
				fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RBP-%d]\n", input_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	BYTE [RSI-%d],	AL\n", insert_list->child->symbol->offset);
			}
			else if(!strcmp(input_list->child->symbol->type, "REAL") && !strcmp(insert_list->child->symbol->type, "REAL"))
			{
				fprintf(fasm, "\t\tMOV	RAX,	QWORD [RBP-%d]\n", input_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	QWORD [RSI-%d],	RAX\n", insert_list->child->symbol->offset);
			}		
		}
		fprintf(fasm, "\n");
		fill_ret_vars(input_list->child->next->next, insert_list->child->next);
		return;
	}
}




void intialise_inp_vars(ast_node* input_list, ast_node* insert_list)
{
	if(input_list==NULL || insert_list==NULL)
		return;

	if((input_list->child->tag==1 && !strcmp(input_list->child->name.l->TOKEN,"ID")) &&  (insert_list->tag==1 && !strcmp(insert_list->name.l->TOKEN,"ID")))
	{
		if(input_list->child->symbol->arr==1 && insert_list->symbol->arr==1)
		{
			fprintf(fasm, "\t\tMOV	RAX,	QWORD [RSI-%d]\n", insert_list->symbol->offset);
			fprintf(fasm, "\t\tMOV	QWORD [RBP-%d],	RAX\n", input_list->child->symbol->offset);
			
			if(input_list->child->symbol->start_arr_dec==1)
			{			
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n",insert_list->symbol->offset - 8);
				fprintf(fasm, "\t\tCMP	AX, 	%d\n",input_list->child->symbol->start_arr);
				fprintf(fasm, "\t\tJNE	ARRAY_PASSING_ERROR\n");
			}
			
			fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->symbol->offset - 8);
			fprintf(fasm, "\t\tMOV	WORD [RBP-%d],	AX\n", input_list->child->symbol->offset - 8);
			
			if(input_list->child->symbol->end_arr_dec==1)
			{			
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n",insert_list->symbol->offset - 10);
				fprintf(fasm, "\t\tCMP	AX, 	%d\n", input_list->child->symbol->end_arr);
				fprintf(fasm, "\t\tJNE	ARRAY_PASSING_ERROR\n");
			}			
			
			fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->symbol->offset - 10);
			fprintf(fasm, "\t\tMOV	WORD [RBP-%d],	AX\n", input_list->child->symbol->offset - 10);	
		}
		else if(!strcmp(input_list->child->symbol->type, "INTEGER") && !strcmp(insert_list->symbol->type, "INTEGER"))
		{
			fprintf(fasm, "\t\tMOV 	EAX, 	DWORD [RSI-%d]\n", insert_list->symbol->offset);
			fprintf(fasm, "\t\tMOV	DWORD [RBP-%d],	EAX\n", input_list->child->symbol->offset);
		}
		else if(!strcmp(input_list->child->symbol->type, "BOOLEAN") && !strcmp(insert_list->symbol->type, "BOOLEAN"))
		{
			fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RSI-%d]\n", insert_list->symbol->offset);
			fprintf(fasm, "\t\tMOV	BYTE [RBP-%d],	AL\n", input_list->child->symbol->offset);
		}
		else if(!strcmp(input_list->child->symbol->type, "REAL") && !strcmp(insert_list->symbol->type, "REAL"))
		{
			fprintf(fasm, "\t\tMOV	RAX,	QWORD [RSI-%d]\n", insert_list->symbol->offset);
			fprintf(fasm, "\t\tMOV	QWORD [RBP-%d],	RAX\n", input_list->child->symbol->offset);
		}
		fprintf(fasm, "\n");
		return;
	}
	else if(input_list->tag==0 &&  insert_list->tag==0)
	{
		if((input_list->child->tag==1 && !strcmp(input_list->child->name.l->TOKEN,"ID")) &&  (insert_list->child->tag==1 && !strcmp(insert_list->child->name.l->TOKEN,"ID")))
		{
			if(input_list->child->symbol->arr==1 && insert_list->child->symbol->arr==1)
			{
				fprintf(fasm, "\t\tMOV	RAX,	QWORD [RSI-%d]\n", insert_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	QWORD [RBP-%d],	RAX\n", input_list->child->symbol->offset);
				
				if(input_list->child->symbol->start_arr_dec==1)
				{			
					fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->child->symbol->offset - 8);
					fprintf(fasm, "\t\tCMP	AX, 	%d\n", input_list->child->symbol->start_arr);
					fprintf(fasm, "\t\tJNE	ARRAY_PASSING_ERROR\n");
				}				
				
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->child->symbol->offset - 8);
				fprintf(fasm, "\t\tMOV	WORD [RBP-%d],	AX\n", input_list->child->symbol->offset - 8);
				
				if(input_list->child->symbol->end_arr_dec==1)
				{			
					fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->child->symbol->offset - 10);
					fprintf(fasm, "\t\tCMP	AX, 	%d\n", input_list->child->symbol->end_arr);
					fprintf(fasm, "\t\tJNE	ARRAY_PASSING_ERROR\n");
				}				
				
				fprintf(fasm, "\t\tMOV 	AX, 	WORD [RSI-%d]\n", insert_list->child->symbol->offset - 10);
				fprintf(fasm, "\t\tMOV	WORD [RBP-%d],	AX\n", input_list->child->symbol->offset - 10);
			}
			else if(!strcmp(input_list->child->symbol->type, "INTEGER") && !strcmp(insert_list->child->symbol->type, "INTEGER"))
			{
				fprintf(fasm, "\t\tMOV 	EAX, 	DWORD [RSI-%d]\n", insert_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	DWORD [RBP-%d],	EAX\n", input_list->child->symbol->offset);
			}
			else if(!strcmp(input_list->child->symbol->type, "BOOLEAN") && !strcmp(insert_list->child->symbol->type, "BOOLEAN"))
			{
				fprintf(fasm, "\t\tMOV 	AL, 	BYTE [RSI-%d]\n", insert_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	BYTE [RBP-%d],	AL\n", input_list->child->symbol->offset);
			}
			else if(!strcmp(input_list->child->symbol->type, "REAL") && !strcmp(insert_list->child->symbol->type, "REAL"))
			{
				fprintf(fasm, "\t\tMOV	RAX,	QWORD [RSI-%d]\n", insert_list->child->symbol->offset);
				fprintf(fasm, "\t\tMOV	QWORD [RBP-%d],	RAX\n", input_list->child->symbol->offset);
			}		
		}
		fprintf(fasm, "\n");
		intialise_inp_vars(input_list->child->next->next, insert_list->child->next);
		return;
	}
}


void PrintTemplateStart()
{
	fprintf(fasm, "\t\t\t\t\tEXTERN		printf\n");
	fprintf(fasm, "\t\t\t\t\tEXTERN		scanf\n\n");
	fprintf(fasm, "\t\t\t\t\tsection		.data\n");		

	fprintf(fasm, "Space		DB			32\n");
	fprintf(fasm, "NewLine		DB			10, 13\n");
	fprintf(fasm, "Minus		DB 			'-'\n\n");
	fprintf(fasm, "terminal 	DB 			'$> '\n");
	fprintf(fasm, "termlen		EQU			3\n\n");
	fprintf(fasm, "input 		DB 			'Input: Enter ', 0\n");
	fprintf(fasm, "iplen		EQU			$-input\n\n");
	fprintf(fasm, "a		DB			'a '\n");
	fprintf(fasm, "alen		EQU			$-a\n\n");
	fprintf(fasm, "an		DB			'an '\n");
	fprintf(fasm, "anlen		EQU			$-an\n\n");
	fprintf(fasm, "integer 	DB 			'integer '\n");
	fprintf(fasm, "intlen		EQU			$-integer\n\n");
	fprintf(fasm, "boolean 	DB 			'boolean '\n");
	fprintf(fasm, "boollen 	EQU			$-boolean\n\n");
	fprintf(fasm, "real 		DB 			'real '\n");
	fprintf(fasm, "realen		EQU			$-real\n\n");
	fprintf(fasm, "value 		DB 			'value', 10\n");
	fprintf(fasm, "vallen		EQU 			$-value\n\n");
	fprintf(fasm, "array 		DB 			' array elements of '\n");
	fprintf(fasm, "arrlen		EQU			$-array\n\n");
	fprintf(fasm, "type		DB			'type for range '\n");
	fprintf(fasm, "typelen		EQU			$-type\n\n");
	fprintf(fasm, "to_		DB			' to '\n");
	fprintf(fasm, "tolen		EQU			$-to_\n\n");
	fprintf(fasm, "output		DB			'Output: '\n");
	fprintf(fasm, "outlen		EQU			$-output\n\n");
	fprintf(fasm, "true		DB			'true'\n");
	fprintf(fasm, "trulen		EQU			$-true	\n\n");
	fprintf(fasm, "false		DB			'false'\n");
	fprintf(fasm, "fallen		EQU			$-false\n\n");
	fprintf(fasm, "Segfault 	DB			'ERROR: Array Index Out of Bounds Accessed.', 10, 13, '$> SEGMENTATION FAULT (core dumped)'\n");
	fprintf(fasm, "Segfaultlen	EQU			$-Segfault\n\n");
	fprintf(fasm, "Array_error	DB			'ERROR: Invalid Array Indices. Memory Allocation Failed.', 10, 13, '$> ABORTED (core dumped)'\n");
	fprintf(fasm, "Arrerrlen	EQU			$-Array_error\n\n");
	fprintf(fasm, "ArrEquErr	DB			'ERROR: Matching Array Bounds. Invalid Array Assignment.', 10, 13, '$> INVALID ASSIGNMENT (execution aborted)'\n");
	fprintf(fasm, "ArrEqulen	EQU 			$-ArrEquErr\n\n");
	fprintf(fasm, "ArrPassErr	DB			'ERROR: Passing Array to Module. Mismatched Bounds cant be passed.', 10, 13, '$> INVALID POINTER (execution aborted)'\n");
	fprintf(fasm, "ArrPasslen	EQU 			$-ArrPassErr\n\n");
	fprintf(fasm, "Input_Error	DB			'ERROR: Accepting Input. Please Enter Input Again.', 10\n");
	fprintf(fasm, "IPErr 		EQU 			$-Input_Error\n\n");
	fprintf(fasm, "ProgEnd		DB			'Program Terminated with exit code 0', 10, 0\n");
	fprintf(fasm, "ProgEndlen	EQU 		$-ProgEnd\n\n");
	fprintf(fasm, "Buffer_len	DB			100\n");
	fprintf(fasm, "InpBuffer	TIMES 			100 DB '$'	\n");
	fprintf(fasm, "OutBuffer	TIMES			100 DB '$'\n\n");
	fprintf(fasm, "Buf		DW			0, 0, 0, 0, 0\n");
	fprintf(fasm, "NumEle		DW			0, 0, 0\n");
	fprintf(fasm, "NumTrav		DW			0, 0\n\n");
	fprintf(fasm, "ModuleSpace	DQ			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \n\n");
	fprintf(fasm, "IntFmt		DB 			\"%%d\", 0\n");
	fprintf(fasm, "FloatScan	DB			\"%%lf\", 0\n");
	fprintf(fasm, "FloatPrint	DB			\"%%e\", 10, 0\n");
	fprintf(fasm, "FloatBuff	DQ			0, 0\n\n");



	fprintf(fasm, "\n\n\n\t\t\t\t\tsection		.text\n\n");
	fprintf(fasm, "global		main\n\n");
	fprintf(fasm, "main:\n\n");
	fprintf(fasm, "\t\tMOV 		EAX,	4\n");
	fprintf(fasm, "\t\tMOV 		EBX,	1\n");
	fprintf(fasm, "\t\tMOV 		ECX, 	NewLine\n");
	fprintf(fasm, "\t\tMOV 		EDX,	2\n");
	fprintf(fasm, "\t\tINT 		80H\n");

}


void PrintTemplateEnd()
{
	fprintf(fasm, "\t\tMOV 		EAX,	4\n");
	fprintf(fasm, "\t\tMOV 		EBX,	1\n");
	fprintf(fasm, "\t\tMOV 		ECX, 	NewLine\n");
	fprintf(fasm, "\t\tMOV 		EDX,	2\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	termlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	terminal\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	ProgEndlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	ProgEnd\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");

	fprintf(fasm, "\n\n\nEXIT:\n");
	fprintf(fasm, "\t\tPOP  		RBP\n");
	fprintf(fasm, "\t\tMOV 		EAX,	4\n");
	fprintf(fasm, "\t\tMOV 		EBX,	1\n");
	fprintf(fasm, "\t\tMOV 		ECX, 	NewLine\n");
	fprintf(fasm, "\t\tMOV 		EDX,	2\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EAX, 	1\n");
	fprintf(fasm, "\t\tMOV		EBX, 	0\n");
	fprintf(fasm, "\t\tINT 		80H\n\n");

	fprintf(fasm, "printEAX:\n");
	fprintf(fasm, "\t\tPUSH		RDI\n");
	fprintf(fasm, "\t\tPUSH		RCX\n");
	fprintf(fasm, "\t\tPUSH		RDX\n");
	fprintf(fasm, "\t\tPUSH		RBX\n");
	fprintf(fasm, "\t\tMOV 		EBX,		80000000H\n");
	fprintf(fasm, "\t\tAND		EBX,		EAX\n");
	fprintf(fasm, "\t\tCMP		EBX,		0\n");
	fprintf(fasm, "\t\tJNE		INVEAX\n");
	fprintf(fasm, "pINV:\n");		
	fprintf(fasm, "\t\tMOV		EDI,		OutBuffer\n");
	fprintf(fasm, "\t\tMOV		ECX,		10\n");
	fprintf(fasm, "\t\tMOV		EDX,		0\n");
	fprintf(fasm, "printAXLoop1:\n");
	fprintf(fasm, "\t\tDIV		ECX\n");
	fprintf(fasm, "\t\tADD		DL,		48\n");
	fprintf(fasm, "\t\tMOV		[EDI],		DL\n");
	fprintf(fasm, "\t\tMOV		EDX,		0\n");
	fprintf(fasm, "\t\tINC 		EDI\n");
	fprintf(fasm, "\t\tCMP		EAX,		0\n");
	fprintf(fasm, "\t\tJNZ		printAXLoop1\n");
	fprintf(fasm, "\t\tSUB		EDI,		1\n");
	fprintf(fasm, "printAXLoop2:\n");
	fprintf(fasm, "\t\tMOV		EDX,		1\n");
	fprintf(fasm, "\t\tMOV		ECX,		EDI\n");
	fprintf(fasm, "\t\tMOV		EBX,		1\n");
	fprintf(fasm, "\t\tMOV		EAX,		4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tDEC		EDI\n");
	fprintf(fasm, "\t\tCMP		EDI,		OutBuffer\n");
	fprintf(fasm, "\t\tJGE		printAXLoop2\n");
	fprintf(fasm, "\t\tCALL		CLR_OUTBUFFER\n");
	fprintf(fasm, "\t\tPOP		RBX\n");
	fprintf(fasm, "\t\tPOP 		RDX\n");
	fprintf(fasm, "\t\tPOP 		RCX\n");
	fprintf(fasm, "\t\tPOP 		RDI\n");
	fprintf(fasm, "\t\tRET\n");
	fprintf(fasm, "INVEAX:\n");
	fprintf(fasm, "\t\tPUSH		RAX\n");
	fprintf(fasm, "\t\tPUSH		RBX\n");
	fprintf(fasm, "\t\tPUSH		RCX\n");
	fprintf(fasm, "\t\tPUSH		RDX\n");
	fprintf(fasm, "\t\tMOV 		EAX,		4\n");
	fprintf(fasm, "\t\tMOV 		EBX,		1\n");
	fprintf(fasm, "\t\tMOV 		ECX, 		Minus\n");
	fprintf(fasm, "\t\tMOV 		EDX,		1\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tPOP 		RDX\n");
	fprintf(fasm, "\t\tPOP 		RCX\n");
	fprintf(fasm, "\t\tPOP 		RBX\n");
	fprintf(fasm, "\t\tPOP 		RAX\n");
	fprintf(fasm, "\t\tXOR		EAX,		0xFFFFFFFF\n");
	fprintf(fasm, "\t\tINC 		EAX\n");
	fprintf(fasm, "\t\tJMP		pINV\n\n\n");


	fprintf(fasm, "CLR_OUTBUFFER:\n");
	fprintf(fasm, "\t\tMOV		BYTE [OutBuffer],		36\n");
	fprintf(fasm, "\t\tMOV		ECX,				100\n");
	fprintf(fasm, "OP_BUFFLOOP:\n");
	fprintf(fasm, "\t\tMOV		BYTE [OutBuffer + ECX],		36\n");
	fprintf(fasm, "\t\tDEC		ECX\n");
	fprintf(fasm, "\t\tJNZ		OP_BUFFLOOP\n");
	fprintf(fasm, "\t\tRET\n\n\n");	
			

	fprintf(fasm, "CLR_IPBUFFER:\n");
	fprintf(fasm, "\t\tMOV		BYTE [InpBuffer],		36\n");
	fprintf(fasm, "\t\tMOV		ECX,		100\n");
	fprintf(fasm, "IP_BUFFLOOP:\n");
	fprintf(fasm, "\t\tMOV		BYTE [InpBuffer + ECX],		36\n");
	fprintf(fasm, "\t\tDEC		ECX\n");
	fprintf(fasm, "\t\tJNZ		IP_BUFFLOOP\n");

	fprintf(fasm, "\t\tRET\n\n\n\n");



	fprintf(fasm, "SEGFAULT_ERROR:\n");
	fprintf(fasm, "\t\tMOV		EDX,	termlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	terminal\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	Segfaultlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	Segfault\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tJMP		EXIT\n\n\n");


	fprintf(fasm, "ARRAY_INDEX_ERROR:\n");
	fprintf(fasm, "\t\tMOV		EDX,	termlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	terminal\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	Arrerrlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	Array_error\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tJMP		EXIT\n\n\n");


	fprintf(fasm, "ARRAY_EQUAL_ERROR:\n");
	fprintf(fasm, "\t\tMOV		EDX,	termlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	terminal\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	ArrEqulen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	ArrEquErr\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tJMP		EXIT\n\n\n");


	fprintf(fasm, "ARRAY_PASSING_ERROR:\n");
	fprintf(fasm, "\t\tMOV		EDX,	termlen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	terminal\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tMOV		EDX,	ArrPasslen\n");
	fprintf(fasm, "\t\tMOV		ECX, 	ArrPassErr\n");
	fprintf(fasm, "\t\tMOV		EBX,	1\n");
	fprintf(fasm, "\t\tMOV		EAX,	4\n");
	fprintf(fasm, "\t\tINT 		80H\n");
	fprintf(fasm, "\t\tJMP		EXIT\n\n\n");
}



