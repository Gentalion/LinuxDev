CFLAGS = -Wall -fPIC

GENERATES = prog prog-a prog-so README
TRASH = *.o *~ o.* *.a *.so
TEST = *.txt

all:	README prog prog-a prog-so

README:	prog
	./$< 2> $@

fun.o:	outlib.h

prog.o:	outlib.h

liboutput_static.a:	const.o fun.o
	ar -rcs $@ $^

liboutput.so: const.o fun.o
	cc -shared $^ -o $@

prog:	const.o fun.o prog.o

prog-a:	prog.o liboutput_static.a
	cc -L. $< -loutput_static -o $@

prog-so: prog.o liboutput.so
	cc -L. $< -loutput -o $@

test: prog prog-a prog-so
	./prog > prog.txt
	./prog-a > prog-a.txt
	LD_LIBRARY_PATH=`pwd` ./prog-so > prog-so.txt

	cmp prog.txt prog-a.txt
	cmp prog-a.txt prog-so.txt
	rm -f $(TEST)

	./prog smth > prog.txt
	./prog-a smth > prog-a.txt
	LD_LIBRARY_PATH=`pwd` ./prog-so smth > prog-so.txt

	cmp prog.txt prog-a.txt
	cmp prog-a.txt prog-so.txt
	rm -f $(TEST)

	./prog a b c > prog.txt
	./prog-a a b c > prog-a.txt
	LD_LIBRARY_PATH=`pwd` ./prog-so a b c > prog-so.txt

	cmp prog.txt prog-a.txt
	cmp prog-a.txt prog-so.txt
	rm -f $(TEST)

	echo "\nTest successful"

clean:
	rm -f $(TRASH)

distclean:	clean
	rm -rf $(GENERATES)