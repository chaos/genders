all:
	javac gov/llnl/lc/chaos/Genders.java
	javac gov/llnl/lc/chaos/GendersException.java
	javac gov/llnl/lc/chaos/GendersExceptionRead.java
	javac gov/llnl/lc/chaos/GendersExceptionParse.java
	javac gov/llnl/lc/chaos/GendersExceptionParameters.java
	javac gov/llnl/lc/chaos/GendersExceptionNotfound.java
	javac gov/llnl/lc/chaos/GendersExceptionSyntax.java
	javac gov/llnl/lc/chaos/GendersExceptionInternal.java
	javac -classpath . GendersTest.java
	javah -o src/Genders.h gov.llnl.lc.chaos.Genders
	gcc src/Genders.c -fPIC -shared -o src/libGenders.so -l genders
	java -Djava.library.path=src/ GendersTest

clean:
	rm -f gov/llnl/lc/chaos/*.class
	rm -f *.class
	rm -f Genders.h
	rm -f libGenders.so
