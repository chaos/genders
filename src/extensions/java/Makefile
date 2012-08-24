all:
#	javac gov/llnl/lc/chaos/Genders.java
#	javac gov/llnl/lc/chaos/GendersException.java
#	javac gov/llnl/lc/chaos/GendersExceptionRead.java
#	javac gov/llnl/lc/chaos/GendersExceptionParse.java
#	javac gov/llnl/lc/chaos/GendersExceptionParameters.java
#	javac gov/llnl/lc/chaos/GendersExceptionNotfound.java
#	javac gov/llnl/lc/chaos/GendersExceptionSyntax.java
#	javac gov/llnl/lc/chaos/GendersExceptionInternal.java
	javac Maintest.java
	javah -o Genders.h gov.llnl.lc.chaos.Genders
	gcc Genders.c -fPIC -shared -o libGenders.so -l genders
	java -D. Maintest
