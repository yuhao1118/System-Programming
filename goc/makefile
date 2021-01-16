.PHONE:ALL
ALL:libgol.so gameoflife

libgol.so:gol.c
	gcc -fPIC -shared -o $@ $^ -Wall -Wextra -pedantic -std=c11
gameoflife:gameoflife.o
	gcc -o $@ -L. $^ -lgol -Wall -Wextra -pedantic -std=c11
gameoflife.o:gameoflife.c
	gcc -l. -o $@ -c $^ -Wall -Wextra -pedantic -std=c11

clean:
	rm -f *.so *.o gameoflife
