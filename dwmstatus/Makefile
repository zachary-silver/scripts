CC=gcc
CFLAGS=-I.
DEPS=dwmstatus.h
OBJ=dwmstatus.o gpmdp.o io.o
LIBS=-lX11 -lasound

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dwmstatus: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm *.o
