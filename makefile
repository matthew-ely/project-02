
compile:
	gcc -Wall -pedantic-errors -o true true.c
	gcc -Wall -pedantic-errors -o false false.c
	gcc -Wall -pedantic-errors -o head head.c
	gcc -Wall -pedantic-errors -o tail tail.c
	gcc -Wall -pedantic-errors -o env env.c
	gcc -Wall -pedantic-errors -o wc wc.c

run: compile
	./head makefile
	./head makefile wc.c
	./tail makefile
	./tail wc.c makefile
	./wc makefile
	./wc makefile wc.c tail.c

clean:
	rm -f true
	rm -f false
	rm -f head
	rm -f tail
	rm -f env
	rm -f wc
	rm -f *.o
