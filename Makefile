all:
	gcc src/*.c -o svm
	./svm test.sapp
util:
	g++ src-util/*.cc -o sasm
	./sasm test.sasm test.sapp