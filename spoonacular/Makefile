CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0 json-c libcurl`

default: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f main
