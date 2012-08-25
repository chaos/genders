if WITH_JAVA_EXTENSIONS
all:
	$(JAVAC) gov/llnl/lc/chaos/Genders.java
	$(JAVAC) gov/llnl/lc/chaos/GendersException.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionOpen.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionRead.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionParse.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionParameters.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionNotfound.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionSyntax.java
	$(JAVAC) gov/llnl/lc/chaos/GendersExceptionInternal.java
	$(CC) src/Genders.c -fPIC -shared -o src/libGenders.so -l genders

genders_header:
	$(JAVAH) -o src/Genders.h gov.llnl.lc.chaos.Genders

test:
	$(JAVAC) -classpath . GendersTest/GendersTest.java
	java -Djava.library.path=src/ -classpath ".:GendersTest" GendersTest

clean:
	rm -f gov/llnl/lc/chaos/*.class
	rm -f src/libGenders.so
	rm -f src/Genders.h
	rm -f GendersTest/*.class
endif
