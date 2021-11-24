CFLAGS=-Wall

all: autoprobe probe1 probe2 test web libnumberpic.so

autoprobe: libnumberpic.so autoprobe.o
	gcc autoprobe.o -o autoprobe -L. -lnumberpic -L/opt/local/lib -lm -Wl,-rpath=.

autoprobe.o: autoprobe.c libnumberpic.h
	gcc -c ${CFLAGS} -g autoprobe.c -o autoprobe.o -I/opt/local/include

probe2: libnumberpic.so probe2.o
	gcc probe2.o -o probe2 -L. -lnumberpic -L/opt/local/lib -lm -Wl,-rpath=.

probe2.o: probe2.c libnumberpic.h
	gcc -c ${CFLAGS} -g probe2.c -o probe2.o -I/opt/local/include

probe1: libnumberpic.so probe1.o
	gcc probe1.o -o probe1 -L. -lnumberpic -L/opt/local/lib -Wl,-rpath=.

probe1.o: probe1.c libnumberpic.h
	gcc -c ${CFLAGS} -g probe1.c -o probe1.o -I/opt/local/include

web:  libnumberpic.so web.o
	gcc web.o -o web -L. -lnumberpic -L/opt/local/lib -Wl,-rpath=.

test: libnumberpic.so test.o
	gcc test.o -o test -L. -lnumberpic -L/opt/local/lib -Wl,-rpath=.

web.o: web.c libnumberpic.h
	gcc -c ${CFLAGS} -g web.c -o web.o -I/opt/local/include

test.o: test.c libnumberpic.h
	gcc -c ${CFLAGS} -g test.c -o test.o -I/opt/local/include

libnumberpic.so: libnumberpic.o
	gcc -shared libnumberpic.o -o libnumberpic.so -L/opt/local/lib -lpng

libnumberpic.o: libnumberpic.c libnumberpic.h
	gcc -fpic -c ${CFLAGS} libnumberpic.c -o libnumberpic.o -I/opt/local/include

clean:
	rm -f test test.o libnumberpic.o libnumberpic.so web.o autoprobe.o autoprobe web probe1.o probe1 probe2.o probe2 autoprobe.o autoprobe
