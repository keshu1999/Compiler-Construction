all:	lexer.o parser.o driver.o ast.o symbolTable.o assembler.o
	gcc -g lexer.o parser.o driver.o ast.o symbolTable.o assembler.o -o compiler
	echo "Compiled to compiler"

lexer.o: lexer.c
	gcc -c -g lexer.c

parser.o: parser.c
	gcc -c -g parser.c

ast.o: ast.c
	gcc -c -g ast.c

symbolTable.o: symbolTable.c
	gcc -c -g symbolTable.c

assembler.o: assembler.c
	gcc -c -g assembler.c

driver.o: driver.c
	gcc -c -g driver.c



run:
	nasm -f elf64 -l code.lst code.asm
	gcc -no-pie -m64 -o code code.o
	echo "RUNNING YOUR PROGRAM"
	./code


clean:
	rm -rf *.o
	rm -rf compiler
	rm -rf code*
