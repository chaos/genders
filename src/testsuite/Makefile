all:
	#to test and debug genders.c
	cp ../libgenders/genders.c .
	cp ../libgenders/genders.h .
	cp ../libgenders/genders_local.h .
	gcc -Wall -o libgenders_test libgenders_test.c genders.c

	#to test the real way
	#gcc -Wall -o libgenders_test libgenders_test.c -lgenders


clean:
	rm libgenders_test
	rm -f *.o
	rm genders.c
	rm genders.h
	rm genders_local.h
