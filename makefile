CFLAGS  = -Wall -g

all  : radius.exe


radius.exe  : boole.o  code.o radius.c  
	gcc $(CFLAGS) $^ -o $@

code.o : code.h code.c
	gcc $(CFLAGS) $^ -c

boole.o : boole.h boole.c
	gcc $(CFLAGS) $^ -c

