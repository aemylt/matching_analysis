CC=gcc
CARGS=-Wall -O3
GMPLIB=-L/gmp_install/lib -lgmp
CMPHLIB=-L/usr/local/lib/libcmph.la -lcmph

all: gen-pattern parameterised-test exact-test

clean: gen-pattern-clean parameterised-test-clean exact-test-clean

parameterised-test:
	$(CC) $(CARGS) -D PARAMETERISED stream_test.c -o parameterised_test

parameterised-test-clean:
	rm parameterised_test

kmp-test:
	$(CC) $(CARGS) -D KMP_TEST stream_test.c -o kmp_test

kmp-test-clean:
	rm kmp_test

exact-test:
	$(CC) $(CARGS) stream_test.c -o exact_test $(GMPLIB) $(CMPHLIB)

exact-test-clean:
	rm exact_test

gen-pattern:
	$(CC) $(CARGS) make_pattern.c -o make_pattern

gen-pattern-clean:
	rm make_pattern