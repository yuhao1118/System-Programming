all:libmaths.so maths_prog

libmaths.so:add.o subtract.o
	gcc -shared -o $@ $^ -Wall -Wextra -pedantic -std=c11
add.o:add.c
	gcc -fPIC -o $@ -c $^ -Wall -Wextra -pedantic -std=c11
subtract.o:subtract.c
	gcc -fPIC -o $@ -c $^ -Wall -Wextra -pedantic -std=c11
maths_prog: maths_prog.o
	gcc $^ -L. -lmaths -o $@ -Wall -Wextra -pedantic -std=c11
maths_prog.o: maths_prog.c
	gcc -o $@ -c $^ -Wall -Wextra -pedantic -std=c11

.PHONY: clean

clean:
	rm -f *.so *.o maths_prog