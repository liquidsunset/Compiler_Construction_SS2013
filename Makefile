default: parser.o
	mkdir -p bin/; gcc -o bin/parser bin/parser.o;./bin/parser

debug: parser.o
	gcc -g src/parser.c -o bin/parser 

%.o: src/%.c
	gcc -c $< -o bin/$@
