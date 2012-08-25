if WITH_JAVA_EXTENSIONS
all:
	javac gov/llnl/lc/chaos/Genders.java
	javac gov/llnl/lc/chaos/GendersException.java
	javac gov/llnl/lc/chaos/GendersExceptionOpen.java
	javac gov/llnl/lc/chaos/GendersExceptionRead.java
	javac gov/llnl/lc/chaos/GendersExceptionParse.java
	javac gov/llnl/lc/chaos/GendersExceptionParameters.java
	javac gov/llnl/lc/chaos/GendersExceptionNotfound.java
	javac gov/llnl/lc/chaos/GendersExceptionSyntax.java
	javac gov/llnl/lc/chaos/GendersExceptionInternal.java
	javah -o src/Genders.h gov.llnl.lc.chaos.Genders
	gcc src/Genders.c -fPIC -shared -o src/libGenders.so -l genders

test:
	javac -classpath . GendersTest/GendersTest.java
	java -Djava.library.path=src/ -classpath ".:GendersTest" GendersTest

clean:
	rm -f gov/llnl/lc/chaos/*.class
	rm -f src/libGenders.so
	rm -f src/Genders.h
	rm -f GendersTest/*.class
endif
