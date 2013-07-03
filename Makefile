default: parser

all: run-parser run-margit

test: ctest run-margit

ctest: parser margit
	./bin/parser test/boolean_expressions.c

run-margit: margit
	./bin/margit

run-parser: parser
	./bin/parser src/scanner.c

margit: margit.o
	gcc -o bin/margit bin/margit.o

parser: parser.o
	gcc -o bin/parser bin/parser.o
	
debug: parser.o margit.o
	gcc -g src/parser.c -o bin/parser 
	gcc -g src/margit.c -o bin/margit

%.o: src/%.c
	mkdir -p bin/
	gcc -c -Wall $< -o bin/$@

clean:
	rm -r bin/
	rm a.out
