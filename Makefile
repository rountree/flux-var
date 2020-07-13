CPPFLAGS=-I../install/include
LDFLAGS=-L../install/lib
CFLAGS=-Wall -std=c11

all: var.so

var.so: var.c
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(CFLAGS) -shared -D BUILD_MODULE -fPIC -o var.so var.c -lflux-core

clean:
	rm var var.so
