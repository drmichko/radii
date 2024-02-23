CFLAGS  = -Wall -g

all  : radius.exe


radius.exe  : boole.o  radius.c  
	gcc $(CFLAGS) $^ -o $@

option.o : option.h option.c
	gcc $(CFLAGS) $^ -c

boole.o : boole.h boole.c
	gcc $(CFLAGS) $^ -c

