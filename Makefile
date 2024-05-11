CC := cproc
CCFLAGS := -lraylib -ldl -lm -lrt -lGL -Wall -O2
OBJS := flap.o

OUTPUT := flap

${OUTPUT} : ${OBJS}
	$(CC) $(CCFLAGS) -o ${OUTPUT} $^

flap.o : flap.c config.h
	$(CC) -c -o $@ flap.c

.PHONY: clean
clean :
	rm ${OBJS} ${OUTPUT}
