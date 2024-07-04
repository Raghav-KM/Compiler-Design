
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

codegen.o = ./build/codegen.o
codegen.h = ./src/headers/codegen.h
codegen.cpp = ./src/codegen.cpp

utils.o = ./build/utils.o
utils.h = ./src/headers/utils.h
utils.cpp = ./src/utils.cpp

testing.o = ./build/testing.o
testing.h = ./src/headers/testing.h
testing.cpp = ./src/testing.cpp

program.asm = ./build/Assembly/program.asm
program.o = ./build/Assembly/program.o
program = ./build/Assembly/program

output = ./build/output
input = ./input/source.dc


output:	${lexer.o} ${parser.o} ${ast.o} ${utils.o} ${codegen.o} ${main.o} ${testing.o}
	g++ $^ -o ${output}

${main.o}: ${main.cpp}
	g++ -c $^ -o ${main.o}

${lexer.o}: ${lexer.cpp} ${lexer.h}
	g++ -c ${lexer.cpp} -o ${lexer.o}

${parser.o}: ${parser.cpp} ${parser.h}
	g++ -c ${parser.cpp} -o ${parser.o}

${ast.o}: ${ast.cpp} ${ast.h}
	g++ -c ${ast.cpp} -o ${ast.o}
	
${codegen.o}: ${codegen.cpp} ${codegen.h}
	g++ -c ${codegen.cpp} -o ${codegen.o}

${utils.o}: ${utils.cpp} ${utils.h}
	g++ -c ${utils.cpp} -o ${utils.o}

${testing.o}: ${testing.cpp} ${testing.h}
	g++ -c ${testing.cpp} -o ${testing.o}

run: output
	${output} ${input}

test: output
	${output} ${input} -t

asm:
	nasm -f elf -o ${program.o} ${program.asm}
	ld -m elf_i386 -o ${program} ${program.o}
	${program}

clean: 
	rm ./build/*.o ${output}
	rm ${program.o} ${program}