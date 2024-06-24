
main.o = ./build/main.o
main.cpp = ./src/main.cpp

lexer.o = ./build/lexer.o
lexer.h = ./src/headers/lexer.h
lexer.cpp = ./src/lexer.cpp

output = ./build/output
input = ./input/source.dc

output:	${main.o} ${lexer.o}
	g++ $^ -o ${output}

${main.o}: ${main.cpp}
	g++ -c $^ -o ${main.o}

${lexer.o}: ${lexer.cpp} ${lexer.h}
	g++ -c ${lexer.cpp} -o ${lexer.o}

run: output
	${output} ${input};

clean: 
	rm ./build/*.o ${output}