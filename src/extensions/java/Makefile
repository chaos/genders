all:
	javac Genders.java Maintest.java
	javah Genders
	gcc Genders.c -fPIC -shared -o libGenders.so -l genders
	java Maintest
