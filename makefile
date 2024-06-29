
main.o = ./build/main.o
main.cpp = ./src/main.cpp

lexer.o = ./build/lexer.o
lexer.h = ./src/headers/lexer.h
lexer.cpp = ./src/lexer.cpp

parser.o = ./build/parser.o
parser.h = ./src/headers/parser.h
parser.cpp = ./src/parser.cpp

ast.o = ./build/ast.o
ast.h = ./src/headers/ast.h
ast.cpp = ./src/ast.cpp

utils.o = ./build/utils.o
utils.h = ./src/headers/utils.h
utils.cpp = ./src/utils.cpp


program.asm = ./build/Assembly/program.asm
program.o = ./build/Assembly/program.o
program = ./build/Assembly/program

output = ./build/output
input = ./input/source.dc

output:	${main.o} ${lexer.o} ${parser.o} ${ast.o} ${utils.o}
	g++ $^ -o ${output}

${main.o}: ${main.cpp}
	g++ -c $^ -o ${main.o}

${lexer.o}: ${lexer.cpp} ${lexer.h}
	g++ -c ${lexer.cpp} -o ${lexer.o}

${parser.o}: ${parser.cpp} ${parser.h}
	g++ -c ${parser.cpp} -o ${parser.o}

${ast.o}: ${ast.cpp} ${ast.h}
	g++ -c ${ast.cpp} -o ${ast.o}

${utils.o}: ${utils.cpp} ${utils.h}
	g++ -c ${utils.cpp} -o ${utils.o}

run: output
	${output} ${input};

asm:
	nasm -f elf -o ${program.o} ${program.asm}
	ld -m elf_i386 -o ${program} ${program.o}
	${program}

clean: 
	rm ./build/*.o ${output}
	rm ${program.o} ${program}