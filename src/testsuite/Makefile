all:
	#to test and debug genders.c
	cp ../libgenders/genders.c .
	cp ../libgenders/genders.h .
	cp ../libgenders/fd.c .
	cp ../libgenders/fd.h .
	cp ../libgenders/list.c .
	cp ../libgenders/list.h .
	gcc -Wall -o libgenders_test libgenders_test.c genders.c fd.c list.c

	#to test the real way
	#gcc -Wall -o libgenders_test libgenders_test.c -lgenders


clean:
	rm libgenders_test
	rm -f *.o
	rm -f genders.c
	rm -f genders.h
	rm -f fd.c
	rm -f fd.h
	rm -f list.c
	rm -f list.h
