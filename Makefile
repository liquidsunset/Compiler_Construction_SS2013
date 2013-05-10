default: parser

margit: margit.o
	mkdir -p bin/; gcc -o bin/margit bin/margit.o; ./bin/margit

parser: parser.o
	mkdir -p bin/; gcc -o bin/parser bin/parser.o; ./bin/parser
	
debug: parser.o
	gcc -g src/parser.c -o bin/parser 

%.o: src/%.c
	gcc -c $< -o bin/$@
