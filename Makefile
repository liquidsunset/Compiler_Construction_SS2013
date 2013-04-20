default: parser.o
	mkdir -p bin/; gcc -o bin/parser bin/parser.o;cd bin/;./parser

%.o: src/%.c
	gcc -c $< -o bin/$@