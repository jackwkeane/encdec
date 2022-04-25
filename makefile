CC=gcc
CFLAGS=-g -c -DDEBUG -Wall -Wextra -Wwrite-strings
LFLAGS=-g

encDec.o: encDec.c
	${CC} ${CFLAGS} encDec.c
encDec_funcs.o: encDec_funcs.c encDec_funcs.h
	${CC} ${CFLAGS} encDec_funcs.c
encDec: encDec.o encDec_funcs.o
	${CC} ${LFLAGS} -o encDec encDec_funcs.o encDec.o
clean:
	rm -f *.o
	rm -f encDec
