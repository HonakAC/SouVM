all:
	gcc src/*.c -o svm
	./svm test.sapp
util:
	g++ src-util/*.cc -o svmt
	./svmt test.sasm test.sapp