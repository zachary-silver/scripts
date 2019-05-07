CC=gcc
CFLAGS=-I.
DEPS = dwm_status.h
OBJ = dwm_status.o gpmdp.o
LIBS=-lX11 -lasound

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dwm_status: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm *.o
