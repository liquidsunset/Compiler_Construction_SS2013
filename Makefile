default: parser

margit: margit.o
	mkdir -p bin/ && gcc -o bin/margit bin/margit.o && ./bin/margit

parser: parser.o
	rm a.out; mkdir -p bin/ && gcc -o bin/parser bin/parser.o && ./bin/parser test/m6.c
	
debug: parser.o
	gcc -g src/parser.c -o bin/parser 

%.o: src/%.c
	gcc -c -Wall $< -o bin/$@
