default: parser

margit: margit.o
	mkdir -p bin/
	gcc -o bin/margit bin/margit.o
	./bin/margit

parser: parser.o
	rm a.out
	mkdir -p bin/
	gcc -o bin/parser bin/parser.o
	./bin/parser test/m6.c
	
debug: parser.o margit.o
	gcc -g src/parser.c -o bin/parser 
	gcc -g src/margit.c -o bin/margit

%.o: src/%.c
	gcc -c -Wall $< -o bin/$@
