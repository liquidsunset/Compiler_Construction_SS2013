scanner: scanner.o
	mkdir -p bin/; gcc -o bin/scanner bin/scanner.o;cd bin/;./scanner

%.o: src/%.c
	gcc -c $< -o bin/$@