all: gen-pattern

clean: gen-pattern-clean

gen-pattern:
	gcc -Wall -O3 make_pattern.c -o make_pattern

gen-pattern-clean:
	rm make_pattern