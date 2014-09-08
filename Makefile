all: gen-pattern test-pattern

clean: gen-pattern-clean test-pattern-clean

test-pattern:
	gcc -Wall -O3 stream_test.c -o stream_test

test-pattern-clean:
	rm stream_test

gen-pattern:
	gcc -Wall -O3 make_pattern.c -o make_pattern

gen-pattern-clean:
	rm make_pattern