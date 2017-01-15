PLAT ?= linux
SHARED := -fPIC --shared
CFLAGS = -g -O2 -Wall

all: vedis

vedis : lua-vedis.c vedis.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o vedis.so

clean:
	rm *.so