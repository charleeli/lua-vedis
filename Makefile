PLAT ?= linux
SHARED := -fPIC --shared
CFLAGS = -g -O2 -Wall

all: lvedis test

lvedis : lua-vedis.c vedis.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o lvedis.so

test : test.c vformat.c sds.c
	$(CC) $(CFLAGS) $^ -o test

clean:
	rm *.so *.vedis test